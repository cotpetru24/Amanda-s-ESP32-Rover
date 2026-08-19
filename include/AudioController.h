#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

class AudioController
{
public:
    void begin();
    void update();

    void playSound(int trackNumber, unsigned long playDrationMs = 0);

private:
    static constexpr int TxPin = 5;
    static constexpr int RxPin = 36;

    HardwareSerial audioSerial{1};
    DFRobotDFPlayerMini player;

    bool timedPlaybackActive = false;
    unsigned long playbackStartedAt = 0;
    unsigned long playbackDurationMs = 0;
}