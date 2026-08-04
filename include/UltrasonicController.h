#pragma once

class UltrasonicController
{
public:
    void begin();
    float getDistanceCentimetres();

private:
    static constexpr int TriggerPin = 16;
    static constexpr int EchoPin = 17;

    static constexpr unsigned long EchoTimeoutMicroseconds = 30000;
};
