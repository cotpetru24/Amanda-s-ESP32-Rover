#pragma once

class StartButtonController
{
public:
    void begin();
    void update();

    bool wasPressed();

private:
    static constexpr int StartButtonPin = 23;
    static constexpr unsigned long DebounceTimeMs = 50;

    bool previousRawState = HIGH;
    bool stableState = HIGH;
    bool pressDetected = false;

    unsigned long lastStateChangeTime = 0;
};
