#ifndef PLANTSENSOR_PLANTSENSOR_H
#define PLANTSENSOR_PLANTSENSOR_H

#include <Arduino.h>

void loop();
void setup();
void readAkku(int);
void readLux();
void readScale();
void readBME();
bool reconnect();
void debug_menu();
void start_ota();
void calibrate_scales();
void startBME();
boolean checkAkku(float);


#endif //PLANTSENSOR_PLANTSENSOR_H
