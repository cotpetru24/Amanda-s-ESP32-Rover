#include <Arduino.h>
#include "StartButtonController.h"

void StartButtonController::begin()
{
    pinMode(StartButtonPin, INPUT_PULLUP);

    previousRawState = digitalRead(StartButtonPin);
    stableState = previousRawState;
}

void StartButtonController::update()
{
    const bool currentRawState = digitalRead(StartButtonPin);

    if (currentRawState != previousRawState)
    {
        previousRawState = currentRawState;
        lastStateChangeTime = millis();
    }

    if (millis() - lastStateChangeTime >= DebounceTimeMs && currentRawState != stableState)
    {
        stableState = currentRawState;

        if (stableState == LOW)
            pressDetected = true;
    }
}

bool StartButtonController::wasPressed()
{
    if (!pressDetected)
        return false;

    pressDetected = false;
    return true;
}