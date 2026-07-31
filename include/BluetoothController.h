#pragma once

class BluetoothController {
public:
    void begin(const char* deviceName);
    bool hasCommand();
    char readCommand();
};