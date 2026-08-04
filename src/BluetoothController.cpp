#include <Arduino.h>
#include "BluetoothController.h"
#include <BluetoothSerial.h>

namespace
{
    BluetoothSerial serialBluetooth;
}

void BluetoothController::begin(const char *deviceName)
{
    serialBluetooth.begin(deviceName);
    Serial.println("Bluetooth started with device name: " + String(deviceName));
}

bool BluetoothController::hasCommand()
{
    return serialBluetooth.available() > 0;
}

char BluetoothController::readCommand()
{
    return static_cast<char>(serialBluetooth.read());
}
