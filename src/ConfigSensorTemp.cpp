#include "ConfigSensorTemp.h"

BME280::TempUnit tempUnit = BME280::TempUnit_Celsius;
BME280::PresUnit presUnit = BME280::PresUnit_bar;
BME280I2C SensorBme;
char tempValueBuffer[16] = "";
char humValueBuffer[16] = "";
char presValueBuffer[16] = "";
float temp(NAN), hum(NAN), pres(NAN);

bool inline startBME() {
    if (!SensorBme.begin())
        return SensorBme.begin();
    return true;
}

void readBME() {
    log_d("Begin reading BME...");
    if (startBME()) {
        SensorBme.read(pres, temp, hum, tempUnit, presUnit);
        // Pressure Exception:
        uint8_t retries = 0;
        while (pres <= 0.65 && retries <= 5) {
            startBME();
            SensorBme.read(pres, temp, hum, tempUnit, presUnit);
            retries++;
        }
        snprintf(tempValueBuffer, sizeof(tempValueBuffer), "%.1f", temp);
        snprintf(humValueBuffer, sizeof(humValueBuffer), "%.1f", hum);
        snprintf(presValueBuffer, sizeof(presValueBuffer), "%.4f", pres);
        log_d("Updating BME: T=%s, H=%s, P=%s", tempValueBuffer, humValueBuffer, presValueBuffer);
    }
}
