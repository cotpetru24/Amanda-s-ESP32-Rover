#include <Arduino.h>
#include "UltrasonicController.h"

void UltrasonicController::begin()
{
    pinMode(TriggerPin, OUTPUT);
    pinMode(EchoPin, INPUT);

    digitalWrite(TriggerPin, LOW);
}

float UltrasonicController::getDistanceCentimetres()
{
    digitalWrite(TriggerPin, LOW);
    delayMicroseconds(2);

    digitalWrite(TriggerPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(TriggerPin, LOW);

    const unsigned long pulseDuration = pulseIn(EchoPin, HIGH, EchoTimeoutMicroseconds);

    if (pulseDuration == 0)
    {
        return -1.0f;
    }

    return pulseDuration * 0.0343f / 2.0f;
}
