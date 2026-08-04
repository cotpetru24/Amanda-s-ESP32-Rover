#include <Arduino.h>
#include "ObstacleSensorController.h"

void ObstacleSensorController::begin()
{
    pinMode(FrontSensorPin, INPUT);
    pinMode(RearSensorPin, INPUT);
}

bool ObstacleSensorController::isFrontObstacleDetected() const
{
    return isObstacleDetected(FrontSensorPin);
};

bool ObstacleSensorController::isRearObstacleDetected() const
{
    return isObstacleDetected(RearSensorPin);
};

bool ObstacleSensorController::isObstacleDetected(std::uint8_t pin) const
{
    return digitalRead(pin) == LOW;
}
