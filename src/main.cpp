#include <Arduino.h>
#include "BluetoothController.h"
#include "MotorController.h"
#include "UltrasonicController.h"
#include "ScannerServoController.h"
#include "NavigationController.h"
#include "StartButtonController.h"

BluetoothController bluetoothController;
MotorController motorController;
UltrasonicController ultrasonicController;
ScannerServoController scannerServoController;
NavigationController navigationController(scannerServoController, ultrasonicController);
StartButtonController startButtonController;

// Rover state
bool canStart = false;
bool autonomousCountdownActive = false;
bool scanResultsPrinted = false;
bool autonomousMode = false;

unsigned long autonomousCountdownStartedAt = 0;

static constexpr unsigned long AutonomousStartDelayMs = 5000;

void stopRover()
{
    canStart = false;
    autonomousCountdownActive = false;
    autonomousMode = false;

    navigationController.begin();

    motorController.emergencyBrake();

    Serial.println("Rover Stopped");
}

void startAutonomousCountdown()
{
    autonomousCountdownActive = true;
    autonomousCountdownStartedAt = millis();

    motorController.emergencyBrake();

    Serial.println("Autonomous mode starts in 5 seconds");
}

void updateAutonomousCountdown()
{
    if (!autonomousCountdownActive)
        return;

    if (millis() - autonomousCountdownStartedAt < AutonomousStartDelayMs)
        return;

    autonomousCountdownActive = false;
    canStart = true;
    autonomousMode = true;
    scanResultsPrinted = false;

    navigationController.startScan();

    Serial.println("Autonomous mode enabled");
}

void handleStartButton()
{
    startButtonController.update();

    if (!startButtonController.wasPressed())
        return;

    if (canStart || autonomousCountdownActive)
    {
        stopRover();
        return;
    }

    startAutonomousCountdown();
}

void processCommand(char command)
{
    switch (command)
    {
    case 'F':
        canStart = true;
        autonomousCountdownActive = false;
        autonomousMode = false;
        navigationController.begin();

        scannerServoController.faceFront();
        motorController.driveForward();
        Serial.println("Driving forward");
        break;

    case 'B':
        canStart = true;
        autonomousCountdownActive = false;
        autonomousMode = false;

        motorController.driveBackward();
        Serial.println("Driving backward");
        break;

    case 'N':
        autonomousMode = false;
        navigationController.begin();
        motorController.coast();
        Serial.println("Coasting");
        break;

    case 'E':
        stopRover();
        Serial.println("Emergency brake activated");
        break;

    case 'L':
        canStart = true;
        autonomousCountdownActive = false;
        autonomousMode = false;

        scannerServoController.faceLeft();
        motorController.steerLeft();
        Serial.println("Steering left");
        break;

    case 'R':
        canStart = true;
        autonomousCountdownActive = false;
        autonomousMode = false;

        scannerServoController.faceRight();
        motorController.steerRight();
        Serial.println("Steering right");
        break;

    case 'C':
        autonomousMode = false;
        navigationController.begin();

        motorController.centerSteering();
        Serial.println("Centering steering");
        break;

    default:
        Serial.print("Unknown command: ");
        Serial.println(command);
    }
}

void enforceObstacleSafety()
{
    if (!canStart ||
        navigationController.isScanning() ||
        (navigationController.isScanComplete() && !scanResultsPrinted))
    {
        return;
    }

    static unsigned long lastCheckTime = 0;

    if (millis() - lastCheckTime < 100)
        return;

    lastCheckTime = millis();

    const float distance = ultrasonicController.getDistanceCentimetres();

    if (navigationController.isObstacleClose(distance))
    {
        motorController.emergencyBrake();

        Serial.println("Obstacle too close - rover stopped");

        if (autonomousMode)
        {
            scanResultsPrinted = false;
            navigationController.startScan();

            Serial.println("Scanning for a clear direction");
        }
        else
        {
            canStart = false;
        }
    }
}

void setup()
{
    Serial.begin(115200);

    startButtonController.begin();
    ultrasonicController.begin();
    motorController.begin();
    scannerServoController.begin();
    navigationController.begin();

    bluetoothController.begin("Amanda-Rover");

    stopRover();
    Serial.println("Amanda Rover ready");
}

void loop()
{
    handleStartButton();

    if (bluetoothController.hasCommand())
    {
        const char command = bluetoothController.readCommand();
        Serial.print("Command received: ");
        Serial.println(command);

        processCommand(command);
    }

    updateAutonomousCountdown();
    enforceObstacleSafety();
    // motorController.updateSteering();

    if (!canStart)
        return;

    navigationController.update();

    static unsigned long lastMeasuredTime = 0;

    // after debugging change to 40ms
    if (!navigationController.isScanning() && millis() - lastMeasuredTime >= 250)
    {
        lastMeasuredTime = millis();
        const float distance = ultrasonicController.getDistanceCentimetres();

        Serial.print("Distance:");

        if (distance < 0)
        {
            Serial.println("No Echo");
        }
        else
        {
            Serial.print(distance);
            Serial.println(" cm");
        }
    }

    if (autonomousMode &&
        navigationController.isScanComplete() &&
        !scanResultsPrinted)
    {
        scanResultsPrinted = true;

        const ScanResult result = navigationController.getScanResult();

        Serial.print("Left: ");
        Serial.print(result.leftDistance);
        Serial.println(" cm");

        Serial.print("Front: ");
        Serial.print(result.frontDistance);
        Serial.println(" cm");

        Serial.print("Right: ");
        Serial.print(result.rightDistance);
        Serial.println(" cm");

        const NavigationDirection bestDirection = navigationController.getBestDirection();

        switch (bestDirection)
        {
        case NavigationDirection::Left:
            Serial.println("Best direction: LEFT");

            motorController.steerLeft();
            delay(500);
            motorController.driveForward();
            break;

        case NavigationDirection::Front:
            Serial.println("Best direction: FRONT");

            scannerServoController.faceFront();
            motorController.driveForward();
            break;

        case NavigationDirection::Right:
            Serial.println("Best direction: RIGHT");

            motorController.steerRight();
            delay(500);
            motorController.driveForward();
            break;

        case NavigationDirection::None:
            Serial.println("No safe direction available");

            motorController.emergencyBrake();
            canStart = false;
            autonomousMode = false;
            break;
        }
    }
