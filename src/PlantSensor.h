#ifndef PLANTSENSOR_PLANTSENSOR_H
#define PLANTSENSOR_PLANTSENSOR_H

#include <Arduino.h>



void loop();
void setup();
void waitUntilPublished();
void setupScales();
void setupScales_();
void readBatteryLevel(int repeatMeasurement);
void readLux();
void setupScales();
void readBME();
bool reconnect(bool connectMqtt);

[[noreturn]] void debug_menu();
void start_ota();
void calibrate_scales();
void startBME();
boolean checkBatteryLevelLow();
boolean checkBatteryLevelWarning();
bool startCloudUpdate(bool fw_version_only);

#endif //PLANTSENSOR_PLANTSENSOR_H
