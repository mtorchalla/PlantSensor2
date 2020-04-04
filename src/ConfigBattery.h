#ifndef PLANTSENSOR_CONFIGBATTERY_H
#define PLANTSENSOR_CONFIGBATTERY_H

#include <Arduino.h>

//Akku
extern char batteryLevelBuffer[16];
extern float batteryLevel;
#define GPIO_Battery_READ A0
#define SHUTDOWN_VOLTAGE 3
//#define R_GND 300000
//#define R_BAT 1000000
//#define R_AKKU (R_BAT + R_GND) / R_GND
//#define O_AKKU 0.3

void inline readBatteryLevel(int repeatMeasurement) {
    batteryLevel = 0;
    for (int i = 1; i < repeatMeasurement; i++) {
        batteryLevel += analogRead(A0);
        delay(1);
    }
    batteryLevel = batteryLevel / repeatMeasurement;
    snprintf(batteryLevelBuffer, sizeof(batteryLevelBuffer), "%.1f", batteryLevel);
    log_d("Reading Battery Level: %s", batteryLevelBuffer);
}

inline float getBatteryVoltage() {
    return batteryLevel / 1000;
}

inline float getBatteryLevel() {
    return (getBatteryVoltage() - 3.0) / 1.2;
}

#endif //PLANTSENSOR_CONFIGBATTERY_H
