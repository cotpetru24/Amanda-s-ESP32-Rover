#include <Arduino.h>
#include "BluetoothController.h"
#include "MotorController.h"

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


