#include <Arduino.h>
#include "NavigationController.h"

NavigationController ::NavigationController(ScannerServoController &scannerServoController, UltrasonicController &ultrasonicController)
    : scannerServo(scannerServoController), ultrasonic(ultrasonicController)
{
}

void NavigationController::begin()
{
    state = ScanState::Idle;
}

void NavigationController::startScan()
{
    if (isScanning())
        return;

    result = {0.0F, 0.0F, 0.0F};

    scannerServo.faceLeft();
    state = ScanState::WaitingForLeft;
    stateStartedAt = millis();

    Serial.println("Navigation scan started");
}

void NavigationController::update()
{
    if (!isCurrentScanPositionSettled())
        return;

    switch (state)
    {
    case ScanState::WaitingForLeft:
        result.leftDistance = measureDistance();

        scannerServo.faceFront();
        state = ScanState::WaitingForFront;
        stateStartedAt = millis();

        break;

    case ScanState::WaitingForFront:
        result.frontDistance = measureDistance();

        scannerServo.faceRight();
        state = ScanState::WaitingForRight;
        stateStartedAt = millis();

        break;

    case ScanState::WaitingForRight:
        result.rightDistance = measureDistance();

        scannerServo.faceFront();
        state = ScanState::Complete;

        Serial.println("Navigation scan complete");

        break;

    case ScanState::Idle:
    case ScanState::Complete:
        break;
    }
};

bool NavigationController::isScanning() const
{
    return state == ScanState::WaitingForRight ||
           state == ScanState::WaitingForLeft ||
           state == ScanState::WaitingForFront;
}

bool NavigationController::isScanComplete() const
{
    return state == ScanState::Complete;
}

ScanResult NavigationController::getScanResult()
{
    return result;
}

NavigationDirection NavigationController::getBestDirection()
{
    if (!isScanComplete())
        return NavigationDirection::None;

    if (result.frontDistance >= result.leftDistance && result.frontDistance >= result.rightDistance)
    {
        return NavigationDirection::Front;
    }

    if (result.leftDistance >= result.rightDistance)
        return NavigationDirection::Left;

    return NavigationDirection::Right;
}

bool NavigationController::isCurrentScanPositionSettled() const
{
    if (!isScanning())
        return false;

    return millis() - stateStartedAt >= ServoSettlingTimeMs;
}

float NavigationController::measureDistance()
{
    const float distance = ultrasonic.getDistanceCentimetres();

    if (distance < 0)
        return MaximumDistanceCm;

    return distance;
}
