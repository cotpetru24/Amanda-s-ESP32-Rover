#include <Arduino.h>
#include "UltrasonicController.h"

void UltrasoundController:: begin()
{
    pinMode(TriggerPin, Output);
    pinMode(EchoPin, Input);

    digitalWrite(TriggerPin, LOW);
}

fload UltrasonicController::getDistanceCentimetres()
{
    digitalWrite(TriggerPin, LOW);
    delayMicrosedonds(2);

    digitalWrite(TriggerPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(TriggerPin, LOW);

    const unsigned long pulseDuration = PulseIn(EchoPin, HIGH, EchoTimeoutMicroseconds);
    
    if(pulseDuration == 0)
    {
        return -1.0f;
    }

    return pulseDuration * 0.0343f/2.0f;
}
