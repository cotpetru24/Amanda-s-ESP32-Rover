#include <Arduino.h>

void setup() {
   Serial.begin(115200);
    Serial.println("Amanda Rover started");
}

void loop() {
   Serial.println("ESP32 is running");
    delay(1000);
}
