#include <Arduino.h>
#include "MotorController.h"

void MotorController::begin() 
{
    pinMode(DriveInput1Pin, OUTPUT);
    pinMode(DriveInput2Pin, OUTPUT);
    pinMode(DriveEnablePin, OUTPUT);

    pinMode(SteeringInput1Pin, OUTPUT);
    pinMode(SteeringInput2Pin, OUTPUT);
    pinMode(SteeringEnablePin, OUTPUT);

    digitalWrite(DriveEnablePin, HIGH);
    digitalWrite(SteeringEnablePin, HIGH);

    emergencyBrake();  // Ensure motors are stopped on startup
    centerSteering();  // Ensure steering is centered on startup
}

void MotorController::driveForward() 
{
    digitalWrite(DriveInput1Pin, HIGH);
    digitalWrite(DriveInput2Pin, LOW);
}

void MotorController::driveBackward() 
{
    digitalWrite(DriveInput1Pin, LOW);
    digitalWrite(DriveInput2Pin, HIGH);
}

void MotorController::coast() 
{
    digitalWrite(DriveInput1Pin, LOW);
    digitalWrite(DriveInput2Pin, LOW);
}

void MotorController::emergencyBrake() 
{
    digitalWrite(DriveInput1Pin, HIGH);
    digitalWrite(DriveInput2Pin, HIGH);
}

void MotorController::steerLeft() 
{
    digitalWrite(SteeringInput1Pin, HIGH);
    digitalWrite(SteeringInput2Pin, LOW);
}

void MotorController::steerRight() 
{
    digitalWrite(SteeringInput1Pin, LOW);
    digitalWrite(SteeringInput2Pin, HIGH);
}

void MotorController::centerSteering() 
{
    digitalWrite(SteeringInput1Pin, LOW);
    digitalWrite(SteeringInput2Pin, LOW);
}