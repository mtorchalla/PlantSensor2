#ifndef PLANTSENSOR_CONFIGSENSORTEMP_H
#define PLANTSENSOR_CONFIGSENSORTEMP_H

#include <Arduino.h>
#include <BME280I2C.h>

//BME:
BME280I2C SensorBme;
bool startup = true;
bool metric = true;
float temp(NAN), hum(NAN), pres(NAN);
char ctemp[16];
char chum[16];
char cpres[16];
BME280::TempUnit tempUnit = BME280::TempUnit_Celsius;
BME280::PresUnit presUnit = BME280::PresUnit_bar;
#define a_len 6 //len of FilterArray
float temp_arr[a_len] = { 0 };
float hum_arr[a_len] = { 0 };
float pres_arr[a_len] = { 0 };
float temp_arr_med[a_len] = { 0 };
float hum_arr_med[a_len] = { 0 };
float pres_arr_med[a_len] = { 0 };
int a_i = 0;
int a_i_last = a_len;

#endif //PLANTSENSOR_CONFIGSENSORTEMP_H
