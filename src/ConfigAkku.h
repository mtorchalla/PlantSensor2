#ifndef PLANTSENSOR_CONFIGAKKU_H
#define PLANTSENSOR_CONFIGAKKU_H

//Akku
extern char cakku[16];
extern float akku;
#define SHUTDOWN_VOLTAGE 3*1000
#define R_GND 300000
#define R_BAT 1000000
#define R_AKKU (R_BAT + R_GND) / R_GND
#define O_AKKU 0.3

#endif //PLANTSENSOR_CONFIGAKKU_H
