#pragma once

#include <Arduino.h>

class MotorController
{
public:

    void begin();

    void driveForward();
    void driveBackward();

    void steerLeft();
    void steerRight();
    void stopSteering();

    void coast();
    void emergencyBrake();

    void setDriveSpeed(uint8_t speed);

private:
    enum class DriveDirection
    {
        Neutral,
        Forward,
        Backward
    };

    enum class SteeringDirection
    {
        Center,
        Left,
        Right
    };

    static constexpr int LeftInput1Pin = 27;
    static constexpr int LeftInput2Pin = 26;
    static constexpr int LeftEnablePin = 25;

    static constexpr int RightInput1Pin = 32;
    static constexpr int RightInput2Pin = 33;
    static constexpr int RightEnablePin = 14;

    static constexpr bool LeftMotorReversed = false;
    static constexpr bool RightMotorReversed = false;

    bool leftMotorReversed;
    bool rightMotorReversed;

    uint8_t driveSpeed = 200;

    DriveDirection driveDirection = DriveDirection::Neutral;
    SteeringDirection steeringDirection = SteeringDirection::Center;

    void applyMovement();

    void setLeftMotor(int16_t speed);
    void setRightMotor(int16_t speed);

    void setMotor(
        uint8_t in1Pin,
        uint8_t in2Pin,
        uint8_t enablePin,
        int16_t speed,
        bool reversed);

    void brakeMotor(
        uint8_t in1Pin,
        uint8_t in2Pin,
        uint8_t enablePin);
};