#pragma once

#include <cstdint>

class ObstacleSensorController
{
    public:
        void begin();
        bool isFrontObstacleDetected() const;
        bool isBackObstacleDetected() const;

    private:
        static constexpr int FrontSensorPin = 18;
        static constexpr int BackSensorPin = 19;

        bool isObstacleDetected(std::uint8_t) const;
};
