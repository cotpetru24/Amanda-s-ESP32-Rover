#pragma once

class MotorController
{
public:
    void begin();

    void driveForward();
    void driveBackward();
    void coast();
    void emergencyBrake();

    void steerLeft();
    void steerRight();
    void centerSteering();

private:
    static constexpr int DriveInput1Pin = 27;
    static constexpr int DriveInput2Pin = 26;
    static constexpr int DriveEnablePin = 25;

    static constexpr int SteeringInput1Pin = 32;
    static constexpr int SteeringInput2Pin = 33;
    static constexpr int SteeringEnablePin = 14;
};