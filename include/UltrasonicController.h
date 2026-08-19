#pragma once

class UltrasonicController
{
public:
    void begin();
    float getDistanceCentimetres();

private:
    static constexpr int TriggerPin = 4;
    static constexpr int EchoPin = 34;

    static constexpr unsigned long EchoTimeoutMicroseconds = 30000;
};
