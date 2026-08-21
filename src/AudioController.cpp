#include "AudioController.h"

void AudioController::begin()
{
    audioSerial.begin(
        9600,
        SERIAL_8N1,
        RxPin,
        TxPin);

    initialized = player.begin(audioSerial);

    if (!initialized)
    {
        Serial.println("DFPlayer initialization failed");
        return;
    }

    player.volume(DefaultVolume);

    Serial.println("DFPlayer ready");
}

void AudioController::update()
{
    if (!timedPlaybackActive)
        return;

    if (millis() - playbackStartedAt < playbackDurationMs)
        return;

    player.stop();
    timedPlaybackActive = false;
}

void AudioController::playTrack(int trackNumber)
{
    if (!initialized)
        return;

    if (trackNumber < 1)
        return;

    currentTrack = trackNumber;
    timedPlaybackActive = false;

    player.playFolder(
        MusicFolder,
        currentTrack);
}

void AudioController::playSound(
    int trackNumber,
    unsigned long playDurationMs)
{
    if (!initialized)
        return;

    if (trackNumber < 1)
        return;

    player.playFolder(
        SystemSoundsFolder,
        trackNumber);

    timedPlaybackActive = playDurationMs > 0;

    if (timedPlaybackActive)
    {
        playbackStartedAt = millis();
        playbackDurationMs = playDurationMs;
    }
}

void AudioController::playNext()
{
    if (!initialized)
        return;

    currentTrack++;

    playTrack(currentTrack);
}

void AudioController::playPrevious()
{
    if (!initialized)
        return;

    if (currentTrack <= 1)
        return;

    currentTrack--;

    playTrack(currentTrack);
}

void AudioController::pause()
{
    if (!initialized)
        return;

    player.pause();
}

void AudioController::resume()
{
    if (!initialized)
        return;

    player.start();
}

void AudioController::stopPlay()
{
    if (!initialized)
        return;

    player.stop();
    timedPlaybackActive = false;
}

void AudioController::setVolume(uint8_t volume)
{
    if (!initialized)
        return;

    player.volume(constrain(volume, 0, 30));
}

void AudioController::volumeUp()
{
    if (!initialized)
        return;

    player.volumeUp();
}

void AudioController::volumeDown()
{
    if (!initialized)
        return;

    player.volumeDown();
}

int AudioController::getCurrentTrackNumber() const
{
    return currentTrack;
}