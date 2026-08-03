#include <Arduino.h>
#include "BluetoothController.h"
#include "MotorController.h"
#include "UltrasonicController.h"
#include "ScannerServoController.h"

BluetoothController bluetoothController;
MotorController motorController;
UltrasonicController ultrasonicController;
ScannerServoController scannerServoController;


void processCommand(char command) {
    switch (command) {
        case 'F':
            scannerServoController.faceFront();
            motorController.driveForward();
            Serial.println("Driving forward");
            break;
        case 'B':
        scannerServoController.faceRear();
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
        scannerServoController.faceLeft();
            motorController.steerLeft();
            Serial.println("Steering left");
            break;
        case 'R':
        scannerServoController.faceLeft();
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

    ultrasonicController.begin();
    motorController.begin();
    scannerServoController.begin();

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

  static unsigned long lastMeasuredTime = 0;

  //after debiggind change to 40ms
  if(millis() - lastMeasuredTime >= 250)
  {
    lastMeasuredTime = millis();
    const float disctance = ultrasonicController.getDistanceCentimetres();

    Serrial.print("Distance:"):

    if(distance < 0)
    {
        Serial.println("No Echo");
    }
    else
    {
        Serial.print(disctance);
        Serial.println(" cm");
    }
  }
}


