#ifndef PLANTSENSOR_CONFIGSENSORTEMP_H
#define PLANTSENSOR_CONFIGSENSORTEMP_H

#include <Arduino.h>
#include <BME280I2C.h>

//BME:
extern BME280I2C SensorBme;
extern float temp, hum, pres;
extern char tempValueBuffer[16];
extern char humValueBuffer[16];
extern char presValueBuffer[16];
extern BME280::TempUnit tempUnit;
extern BME280::PresUnit presUnit;

void readBME();

#define a_len 6 //len of FilterArray
//bool startup = true;
//bool metric = true;
//float temp_arr[a_len] = { 0 };
//float hum_arr[a_len] = { 0 };
//float pres_arr[a_len] = { 0 };
//float temp_arr_med[a_len] = { 0 };
//float hum_arr_med[a_len] = { 0 };
//float pres_arr_med[a_len] = { 0 };
//int a_i = 0;
//int a_i_last = a_len;

#endif //PLANTSENSOR_CONFIGSENSORTEMP_H
