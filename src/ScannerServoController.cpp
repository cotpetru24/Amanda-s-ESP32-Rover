#include <Arduino.h>
#include "ScannerServoController.h"

void ScannerServoController::begin()
{
    scannerServo.setPeriodHertz(50);

    scannerServo.attach(ServoPin, MinPulseWidth, MaxpulseWigth);

    faceCentre();
}

void ScannerServoController::moveToAngle(int angle)
{
    const int safeAngle = constrain(angle, 1, 180);
    scannerServo.write(safeAngle);
    currentAngle = safeAngle;
}

void ScannerServoController::faceFront()
{
    moveToAngle(FrontAngle);
}


void ScannerServoController::faceRight()
{
    moveToAngle(RightAngle);
}

void ScannerServoController::faceLeft()
{
    moveToAngle(LeftAngle);
}

void ScannerServoController::faceCentre()
{
    moveToAngle(CentreAngle);
}

int ScannerServoController::getCurrentAngle() const
{
    return currentAngle;
}