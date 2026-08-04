#pragma once

#include <cstdint>

class ObstacleSensorController
{
public:
    void begin();
    bool isFrontObstacleDetected() const;
    bool isRearObstacleDetected() const;

private:
    static constexpr int FrontSensorPin = 18;
    static constexpr int RearSensorPin = 19;

    bool isObstacleDetected(std::uint8_t) const;
};
