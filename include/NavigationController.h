#pragma once

#include "ScannerServoController.h"
#include "UltrasonicController.h"

enum class NavigationDirection
{
    Front,
    Right,
    Left,
    None

};

struct ScanResult
{
    float frontDistance;
    float rightDistance;
    float leftDistance;
};

class NavigationController
{
public:
    NavigationController(ScannerServoController &ScannerServoController,
                         UltrasonicController &UltrasonicController);

    void begin();
    void startScan();
    void update();

    bool isScanning() const;
    bool isScanComplete() const;

    bool isObstacleAhead(float distance) const;
    bool isObstacleClose(float distance) const;

    ScanResult getScanResult();
    NavigationDirection getBestDirection();

private:
    enum class ScanState
    {
        Idle,
        WaitingForFront,
        WaitingForRight,
        WaitingForLeft,
        WaitingForReturnToFront,
        Complete
    };

    static constexpr unsigned long ServoSettlingTimeMs = 400;
    static constexpr float MaximumDistanceCm = 400.0F;

    static constexpr float STOP_DISTANCE_CM = 20.20F;
    static constexpr float WARNING_DISTANCE_CM = 40.0F;
    
    ScannerServoController &scannerServo;
    UltrasonicController &ultrasonic;

    ScanState state = ScanState::Idle;
    ScanResult result{0.0F, 0.0F, 0.0F};

    unsigned long stateStartedAt = 0;

    bool isCurrentScanPositionSettled() const;
    float measureDistance();
};