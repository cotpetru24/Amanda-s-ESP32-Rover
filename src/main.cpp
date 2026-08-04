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


void processCommand(char command) {
    switch (command) {
        case 'F':
            motorController.driveForward();
            Serial.println("Driving forward");
            break;
        case 'B':
            motorController.driveBackward();
            Serial.println("Driving backward");
            break;
        case 'N':
            motorController.coast();
            Serial.println("Coasting");
            break;
        case 'E':
            motorController.emergencyBrake();
            Serial.println("Emergency brake activated");
            break;
        case 'L':
            motorController.steerLeft();
            Serial.println("Steering left");
            break;
        case 'R':
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



void setup() {
   Serial.begin(115200);

    startButtonController.begin();
    ultrasonicController.begin();
    motorController.begin();
    bluetoothController.begin("Amanda-Rover");
    Serial.println("Amanda Rover ready");
}

void loop() {
  if(!bluetoothController.hasCommand()) {
    return;
  }

  const char command = bluetoothController.readCommand();
  Serial.print("Command received: ");
  Serial.println(command);
  
  processCommand(command);
}

    updateAutonomousCountdown();

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
            break;

        case NavigationDirection::Front:
            Serial.println("Best direction: FRONT");
            break;

        case NavigationDirection::Right:
            Serial.println("Best direction: RIGHT");
            break;

        case NavigationDirection::None:
            Serial.println("Best direction: NONE");
            break;
        }
    }
}
