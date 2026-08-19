#pragma once

class StartButtonController
{
public:
    void begin();
    void update();
    bool wasPressed();

private:
    static constexpr int StartButtonPin = 35;
    static constexpr unsigned long DebounceTimeMs = 50;

    bool previousRawState = true;
    bool stableState = true;
    bool pressDetected = false;

    unsigned long lastStateChangeTime = 0;
};