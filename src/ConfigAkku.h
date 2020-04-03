#ifndef PLANTSENSOR_CONFIGAKKU_H
#define PLANTSENSOR_CONFIGAKKU_H

#include <Arduino.h>

//Akku
extern char cakku[16];
extern float akku;
#define GPIO_AKKU_READ A0
#define SHUTDOWN_VOLTAGE 3*1000
#define R_GND 300000
#define R_BAT 1000000
#define R_AKKU (R_BAT + R_GND) / R_GND
#define O_AKKU 0.3

void inline readAkku(int iter) {
    akku = 0;
    for (int i = 1; i < iter; i++) {
        akku += analogRead(A0);
        delay(5);
    }
    akku = akku / iter;
    snprintf(cakku, sizeof(cakku), "%.1f", akku);
#ifdef debug
    Serial.print("Akku: ");
	Serial.println(cakku);
#endif
}

#endif //PLANTSENSOR_CONFIGAKKU_H
