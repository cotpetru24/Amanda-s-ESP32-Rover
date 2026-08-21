#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

class AudioController
{
public:
    void begin();
    void update();

    void playTrack(int trackNumber = 1);
    void playSound(int trackNumber = 1, unsigned long playDurationMs = 0);

    void playNext();
    void playPrevious();

    void pause();
    void resume();
    void stopPlay();

    void setVolume(uint8_t volume);
    void volumeUp();
    void volumeDown();

    int getCurrentTrackNumber() const;
    const char *getCurrentTrackName() const;

private:
    static constexpr int TxPin = 17;
    static constexpr int RxPin = 36;

    static constexpr int MusicFolder = 1;
    static constexpr int SystemSoundsFolder = 2;

    static constexpr uint8_t DefaultVolume = 20;

    HardwareSerial audioSerial{1};
    DFRobotDFPlayerMini player;

    bool initialized = false;

    int currentTrack = 1;

    bool timedPlaybackActive = false;
    unsigned long playbackStartedAt = 0;
    unsigned long playbackDurationMs = 0;

    int getMusicTrackCount() const;
};