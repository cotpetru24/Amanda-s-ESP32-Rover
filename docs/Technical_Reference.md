# Amanda ESP32 Rover — Technical Reference

> Current software/hardware reference for the rover project.  
> Update this file whenever GPIO assignments, sensor thresholds, or hardware modules change.

## ESP32 GPIO Pinout

| Function | GPIO | Notes |
|---|---:|---|
| Left motor IN1 | 27 | Left track direction |
| Left motor IN2 | 26 | Left track direction |
| Left motor Enable / PWM | 25 | Left track speed control |
| Right motor IN1 | 32 | Right track direction |
| Right motor IN2 | 33 | Right track direction |
| Right motor Enable / PWM | 14 | Right track speed control |
| Ultrasonic Trigger | 21 | HC-SR04 TRIG |
| Ultrasonic Echo | 35 | HC-SR04 ECHO — must be reduced to ESP32-safe voltage |
| Scanner servo signal | 13 | Ultrasonic scanning servo |
| Start button | 23 | Uses `INPUT_PULLUP` |
| OLED SDA | 18 | I2C data |
| OLED SCL / SCK | 19 | I2C clock |

## Motor / Track Control

The rover uses two independently driven tracks with pivot/skid steering.

### Movement

| Command | Left Track | Right Track |
|---|---|---|
| Forward | Forward | Forward |
| Backward | Backward | Backward |
| Pivot left | Backward | Forward |
| Pivot right | Forward | Backward |
| Coast | Off | Off |
| Emergency brake | Brake | Brake |

### Motor Constants

```cpp
static constexpr int LeftInput1Pin = 27;
static constexpr int LeftInput2Pin = 26;
static constexpr int LeftEnablePin = 25;

static constexpr int RightInput1Pin = 32;
static constexpr int RightInput2Pin = 33;
static constexpr int RightEnablePin = 14;

static constexpr bool LeftMotorReversed = false;
static constexpr bool RightMotorReversed = false;

uint8_t driveSpeed = 200;
```

### Motor Direction Calibration

Do **not** change the reversed flags until the tracks are physically tested.

When `driveForward()` is commanded:

- both tracks must propel the rover forward;
- if only the left track runs backwards, set `LeftMotorReversed = true`;
- if only the right track runs backwards, set `RightMotorReversed = true`.

Do not change the movement logic or swap direction logic just to correct motor orientation.

---

## Ultrasonic Sensor — HC-SR04

| Setting | Value |
|---|---:|
| Trigger GPIO | 21 |
| Echo GPIO | 35 |
| Echo timeout | 30,000 µs |
| Maximum navigation distance | 400 cm |
| Warning distance | 40 cm |
| Stop distance | 20.20 cm |

Distance calculation:

```cpp
distanceCm = pulseDuration * 0.0343F / 2.0F;
```

A failed echo returns:

```cpp
-1.0F
```

During navigation scanning, a failed reading is currently treated as:

```cpp
400.0F
```

### Important

HC-SR04 ECHO is a 5 V signal. The ESP32 GPIO must not receive 5 V directly. Use the voltage divider fitted in the hardware.

---

## Scanner Servo

| Setting | Value |
|---|---:|
| Signal GPIO | 13 |
| Servo frequency | 50 Hz |
| Minimum pulse width | 500 µs |
| Maximum pulse width | 2400 µs |
| Left angle | 45° |
| Centre angle | 90° |
| Right angle | 135° |
| Front angle | 170° |
| Servo settling time | 400 ms |

Rear scanning has been removed.

Navigation scan order:

```text
LEFT -> FRONT -> RIGHT -> RETURN TO FRONT
```

---

## OLED Display

Module:

```text
0.96 inch OLED
SSD1306
128 x 64
I2C
```

### Wiring

| OLED Pin | ESP32 |
|---|---|
| GND | GND |
| VDD | 3.3 V |
| SDA | GPIO18 |
| SCK / SCL | GPIO19 |

### Display Constants

```cpp
static constexpr int ScreenWidth = 128;
static constexpr int ScreenHeight = 64;

static constexpr int SdaPin = 18;
static constexpr int SclPin = 19;

static constexpr int ResetPin = -1;
static constexpr uint8_t DisplayAddress = 0x3C;
```

The I2C bus is configured with:

```cpp
Wire.begin(SdaPin, SclPin);
```

The display uses the same `Wire` bus:

```cpp
Adafruit_SSD1306 display{
    ScreenWidth,
    ScreenHeight,
    &Wire,
    ResetPin
};
```

Libraries:

```text
Adafruit GFX Library
Adafruit SSD1306
```

If the OLED does not respond at `0x3C`, check its I2C address before changing other display code.

---

## Start Button

| Setting | Value |
|---|---:|
| GPIO | 23 |
| Input mode | `INPUT_PULLUP` |
| Debounce | 50 ms |

The button starts the autonomous-mode countdown. Pressing it again while running/counting down stops the rover.

---

## Bluetooth

Bluetooth device name:

```text
Amanda-Rover
```

Current commands:

| Command | Action |
|---|---|
| `F` | Forward |
| `B` | Backward |
| `L` | Pivot left |
| `R` | Pivot right |
| `C` | Stop turning / return to straight movement state |
| `N` | Coast |
| `E` | Emergency stop |

Manual commands cancel autonomous mode.

---

## Autonomous Navigation

Autonomous start delay:

```cpp
static constexpr unsigned long AutonomousStartDelayMs = 5000;
```

Navigation behaviour:

```text
START
  |
  v
5 second countdown
  |
  v
SCAN LEFT / FRONT / RIGHT
  |
  v
Choose the safe direction with the greatest measured distance
  |
  v
Drive
  |
  v
Obstacle <= 20.20 cm
  |
  v
Emergency brake
  |
  v
Scan again
  |
  v
Choose new direction
```

If LEFT, FRONT and RIGHT are all blocked within the stop distance:

```text
STOP -> PIVOT 180° -> STOP -> SCAN AGAIN
```

### Temporary Pivot Timing

Current temporary timing:

```text
Normal left/right navigation pivot: 500 ms
180° pivot:                         1000 ms
```

These values **must be calibrated after the rover is physically assembled**.

The blocking `delay()` calls should later be replaced with non-blocking `millis()`-based pivot timing.

---

## Serial Debugging

```cpp
Serial.begin(115200);
```

Serial baud rate:

```text
115200
```

---

## Current Controllers

```text
MotorController
BluetoothController
UltrasonicController
ScannerServoController
NavigationController
StartButtonController
DisplayController
```

Planned later:

```text
RoverController
PirController
BatteryController
Camera / ESP32-CAM software
Wi-Fi / Web server
GuardModeController
Audio / voice controller
Laser controller
SolarTrackerController
Telemetry / status
Failsafe / watchdog logic
```

IR obstacle sensors/controllers have been removed from the design.

---

## Power / Hardware Notes

Current rover power concept:

- 2S battery pack.
- Battery protection / charging module with `B-`, `BM`, and `B+`.
- LM2596 buck converter for the regulated 5 V rail.
- Common ground bus between ESP32, sensors, motor control electronics and power system.
- Main battery positive passes through fuse and main power switch.
- Motors use suppression capacitors.
- Motor-driver supply should have a bulk capacitor close to the driver.

---

## Important Checks Before First Drive

1. Verify both tracks move forward when `driveForward()` is called.
2. Change only `LeftMotorReversed` or `RightMotorReversed` if required.
3. Verify pivot-left and pivot-right directions.
4. Calibrate 90° and 180° pivot timing.
5. Verify ultrasonic distance readings before enabling autonomous driving.
6. Verify the HC-SR04 ECHO divider before connecting the sensor.
7. Verify the OLED I2C address.
8. Confirm scanner LEFT / FRONT / RIGHT angles physically match the rover.
9. Test emergency braking.
10. Remove temporary blocking `delay()` calls after movement calibration.
