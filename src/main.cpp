#include <Arduino.h>
#include "BluetoothController.h"
#include "MotorController.h"
#include "UltrasonicController.h"
#include "ScannerServoController.h"
#include "NavigationController.h"
#include "StartButtonController.h"
#include "ObstacleSensorController.h"

BluetoothController bluetoothController;
MotorController motorController;
ObstacleSensorController obstacleSensorController;
UltrasonicController ultrasonicController;
ScannerServoController scannerServoController;
NavigationController navigationController(scannerServoController, ultrasonicController);
StartButtonController startButtonController;

// Rover state
bool canStart = false;
bool autonomousCountdownActive = false;
unsigned long autonomousCountdownStartedAt = 0;

static constexpr unsigned long AutonomousStartDelayMs = 5000;

void stopRover()
{
    canStart = false;
    autonomousCountdownActive = false;

    motorController.emergencyBrake();

    // not sure we need this!!!
    motorController.centerSteering();

    Serial.println("Rover Stopped");
}

void startAutonomousCountdown()
{
    autonomousCountdownActive = true;
    autonomousCountdownStartedAt = millis();

    motorController.emergencyBrake();
    // not sure if we need this
    motorController.centerSteering();

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

        scannerServoController.faceFront();
        motorController.driveForward();
        Serial.println("Driving forward");
        break;

    case 'B':
        canStart = true;
        autonomousCountdownActive = false;

        scannerServoController.faceRear();
        motorController.driveBackward();
        Serial.println("Driving backward");
        break;

    case 'N':
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

        scannerServoController.faceLeft();
        motorController.steerLeft();
        Serial.println("Steering left");
        break;

    case 'R':
        canStart = true;
        autonomousCountdownActive = false;

        scannerServoController.faceRight();
        motorController.steerRight();
        Serial.println("Steering right");
        break;

    case 'C':
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
    const DriveState driveState = motorController.getDriveState();

    if (
        driveState == DriveState::Forward &&
        obstacleSensorController.isFrontObstacleDetected())
    {
        motorController.emergencyBrake();
        Serial.println("Emergency stop: front obstacle");
        return;
    }

    if (
        driveState == DriveState::Reverse &&
        obstacleSensorController.isRearObstacleDetected())
    {
        motorController.emergencyBrake();
        Serial.println("Emergency stop: rear obstacle");
    }
}

void setup()
{
    Serial.begin(115200);

    startButtonController.begin();
    obstacleSensorController.begin();
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
    motorController.updateSteering();

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

    static bool scanResultsPrinted = false;
    if (navigationController.isScanComplete() && !scanResultsPrinted)
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
        motorController.driveForward();
        break;

    case NavigationDirection::None:
        Serial.println("Best direction: NONE");

        motorController.emergencyBrake();
        break;
}
    }
}
