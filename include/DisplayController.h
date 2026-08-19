#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayController
{
public:
    void begin();

    void showReady();
    void showMessage(const char *title, const char *message);
    void showStatus(const char *mode, const char *movement, float distance);

private:
    static constexpr int ScreenWidth = 128;
    static constexpr int ScreenHeight = 64;

    static constexpr int SdaPin = 21;
    static constexpr int SclPin = 22;

    static constexpr int ResetPin = -1;
    static constexpr uint8_t DisplayAddress = 0x3C;

    Adafruit_SSD1306 display { ScreenWidth, ScreenHeight, &Wire, ResetPin };

    bool initialised = false;
};