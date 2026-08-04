#pragma once

enum class DriveState
{
    Stopped,
    Forward,
    Reverse
};

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
    // void updateSteering();

    DriveState getDriveState() const;

private:
    DriveState currentDriveState = DriveState::Stopped;

    static constexpr int DriveInput1Pin = 27;
    static constexpr int DriveInput2Pin = 26;
    static constexpr int DriveEnablePin = 25;

    static constexpr int SteeringInput1Pin = 32;
    static constexpr int SteeringInput2Pin = 33;
    static constexpr int SteeringEnablePin = 14;

    // static constexpr unsigned long SteeringPulseTimeMs = 800;
    
    // bool steeringActive = false;
    // unsigned long steeringStartedAt = 0;
};