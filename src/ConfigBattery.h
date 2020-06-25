#ifndef PLANTSENSOR_CONFIGBATTERY_H
#define PLANTSENSOR_CONFIGBATTERY_H

#include <Arduino.h>
#include "Pins.h"

//Akku
extern char batteryLevelBuffer[16];
extern float batteryLevel;
#define GPIO_Battery_READ PIN_VBAT_UPDATE
#define SHUTDOWN_VOLTAGE 3
#define R_GND 1000000
#define R_BAT 474000
//#define R_AKKU R_GND / (R_BAT + R_GND)
#define R_AKKU 737.0f/500.0f
//#define O_AKKU 0.3

void inline readBatteryLevel(int repeatMeasurement) {
    batteryLevel = 0;
    for (int i = 1; i < repeatMeasurement; i++) {
        batteryLevel += analogRead(PIN_VBAT_UPDATE);
        delay(1);
    }
    batteryLevel = batteryLevel / repeatMeasurement;
    log_d("Raw battery Level: %f", batteryLevel);
    batteryLevel = batteryLevel * R_AKKU;
    snprintf(batteryLevelBuffer, sizeof(batteryLevelBuffer), "%.1f", batteryLevel);
    log_d("Read Battery Level: %s", batteryLevelBuffer);
}

inline float getBatteryVoltage() {
    return batteryLevel / 1000;
}

inline float getBatteryLevel() {
    return (getBatteryVoltage() - 3.0) / 1.2;
}

#endif //PLANTSENSOR_CONFIGBATTERY_H
