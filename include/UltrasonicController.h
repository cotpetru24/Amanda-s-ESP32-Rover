#pragma once

class UltrasonicController
{
public:
    void begin();
    float getDistanceCentimetres();

private:
    static constexpr int TriggerPin = 34;
    static constexpr int EchoPin = 35;

    static constexpr unsigned long EchoTimeoutMicroseconds = 30000;
};
