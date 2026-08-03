#pragma once

#include <ESP32Servo.h>

class ScannerServoController
{
    public:
        void begin();
        void moveToAngle();

        void faceFront();
        void faceRear();
        void faceLeft();
        void faceRight();
        void faceCentre();

        int getCurrentAngle() const;

    private:
        static constexpr int ServoPin = 13;

        static constexpr int RearAngle = 10;
        static constexpr int LeftAngle = 45;
        static constexpr int CentreAngle = 90;
        static constexpr int RightAngle = 135;
        static constexpr int FrontAngle = 170;

        static constexpr int MinPulseWidth = 500;
        static constexpr int MaxpulseWigth = 2400;

        Servo scannerServo;
        int currentAngle = CentreAngle;

}