#include <Arduino.h>
#include "ObstacleSensorController.h"

void ObstacleSensorController::begin()
{
    pinMode(FrontSensorPin, INPUT);
    pinMode(BackSensorPin, INPUT);
}

bool ObstacleSensorController::isFrontObstacleDetected() const
{
    return isObstacleDetected(FrontSensorPin);
};

bool ObstacleSensorController::isBackObstacleDetected() const
{
    return isObstacleDetected(BackSensorPin);
};

bool ObstacleSensorController::isObstacleDetected(std::uint8_t pin) const
{
    return digitalRead(pin) == LOW;
} 
