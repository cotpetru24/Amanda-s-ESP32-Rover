#include "AudioController.h"

void AudioController::begin()
{
    audioSerial.begin(9600, SERIAL_8N1, RxPin, TxPin);

    if(!player.begin(audioSerial)) return;
    player.volume(20);
}

void AudioController::update()
{
    if(!timedPlaybackActive) return;

    if(millis() - playbackStartedAt >= playbackDurationMs)
    {
        player.stop();
        timedPlaybackActive = false;
    }


}

void AudioController::playSound(int trackNumber, unsigned long playDurationMs)
{
    player.play(trackNumber);

    timedPlaybackActive = playDurationMs > 0;

    if(timedPlaybackActive)
    {
        playbackStartedAt = millis();
        playbackDurationMs = playDurationMs;
    }
}