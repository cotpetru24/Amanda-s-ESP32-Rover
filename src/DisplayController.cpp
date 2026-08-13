#include "DisplayController.h"

void DisplayController::begin()
{
    Wire.begin(SdaPin, SclPin);

    initialised = display.begin(SSD1306_SWITCHCAPVCC, DisplayAddress);

    if(!initialised)
    {
        Serial.println("Display initialisation failed");
        return;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    showReady();
}

void DisplayController::showReady()
{
    if(!initialised) return;

    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0,10);
    display.println("Wall-E");

    display.setTextSize(1);
    display.setCursor(0,35);
    display.println("Rover ready");

    display.display();
}

void DisplayController::showMessage(const char *title, const char *message)
{
    if (!initialised) return;

    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(0, 5);
    display.println(title);

    display.setCursor(0, 25);
    display.println(message);

    display.display();
}

void DisplayController::showStatus(const char *mode, const char *movement, float distanceCm)
{
    if (!initialised) return;

    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("Mode: ");
    display.println(mode);

    display.setCursor(0, 20);
    display.print("Move: ");
    display.println(movement);

    display.setCursor(0, 40);
    display.print("Distance: ");

    if (distanceCm < 0)
    {
        display.println("--");
    }
    else
    {
        display.print(distanceCm, 0);
        display.println(" cm");
    }

    display.display();
}