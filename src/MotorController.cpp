#include "MotorController.h"

void MotorController::begin()
{
    pinMode(LeftInput1Pin, OUTPUT);
    pinMode(LeftInput2Pin, OUTPUT);
    pinMode(LeftEnablePin, OUTPUT);

    pinMode(RightInput1Pin, OUTPUT);
    pinMode(RightInput2Pin, OUTPUT);
    pinMode(RightEnablePin, OUTPUT);

    coast();
}

void MotorController::driveForward()
{
    driveDirection = DriveDirection::Forward;
    steeringDirection = SteeringDirection::Center;

    applyMovement();
}

void MotorController::driveBackward()
{
    driveDirection = DriveDirection::Backward;
    steeringDirection = SteeringDirection::Center;

    applyMovement();
}

void MotorController::steerLeft()
{
    steeringDirection = SteeringDirection::Left;

    applyMovement();
}

void MotorController::steerRight()
{
    steeringDirection = SteeringDirection::Right;

    applyMovement();
}

void MotorController::centerSteering()
{
    steeringDirection = SteeringDirection::Center;

    applyMovement();
}

void MotorController::coast()
{
    driveDirection = DriveDirection::Neutral;
    steeringDirection = SteeringDirection::Center;

    setLeftMotor(0);
    setRightMotor(0);
}

void MotorController::emergencyBrake()
{
    driveDirection = DriveDirection::Neutral;
    steeringDirection = SteeringDirection::Center;

    brakeMotor(
        LeftInput1Pin,
        LeftInput2Pin,
        LeftEnablePin);

    brakeMotor(
        RightInput1Pin,
        RightInput2Pin,
        RightEnablePin);
}

void MotorController::setDriveSpeed(uint8_t speed)
{
    driveSpeed = speed;

    applyMovement();
}

void MotorController::applyMovement()
{
    if (steeringDirection == SteeringDirection::Left)
    {
        setLeftMotor(-driveSpeed);
        setRightMotor(driveSpeed);

        return;
    }

    if (steeringDirection == SteeringDirection::Right)
    {
        setLeftMotor(driveSpeed);
        setRightMotor(-driveSpeed);

        return;
    }

    if (driveDirection == DriveDirection::Forward)
    {
        setLeftMotor(driveSpeed);
        setRightMotor(driveSpeed);

        return;
    }

    if (driveDirection == DriveDirection::Backward)
    {
        setLeftMotor(-driveSpeed);
        setRightMotor(-driveSpeed);

        return;
    }

    setLeftMotor(0);
    setRightMotor(0);
}

void MotorController::setLeftMotor(int16_t speed)
{
    setMotor(
        LeftInput1Pin,
        LeftInput2Pin,
        LeftEnablePin,
        speed,
        LeftMotorReversed);
}

void MotorController::setRightMotor(int16_t speed)
{
    setMotor(
        RightInput1Pin,
        RightInput2Pin,
        RightEnablePin,
        speed,
        RightMotorReversed);
}

void MotorController::setMotor(
    uint8_t in1Pin,
    uint8_t in2Pin,
    uint8_t enablePin,
    int16_t speed,
    bool reversed)
{
    speed = constrain(speed, -255, 255);

    if (reversed)
    {
        speed = -speed;
    }

    if (speed > 0)
    {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);

        analogWrite(enablePin, speed);
    }
    else if (speed < 0)
    {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);

        analogWrite(enablePin, -speed);
    }
    else
    {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);

        analogWrite(enablePin, 0);
    }
}

void MotorController::brakeMotor(
    uint8_t in1Pin,
    uint8_t in2Pin,
    uint8_t enablePin)
{
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);

    analogWrite(enablePin, 255);
}