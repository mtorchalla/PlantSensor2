#include "ConfigLux.h"

Max44009 SensorLux(0x4A, 4, 5);
float luxValue;
char luxValueBuffer[16];

void readLux() {
    luxValue = 0;
    if (SensorLux.getError() == 0) {
        for (int i = 1; i < luxMid; i++) {
            luxValue += SensorLux.getLux();
            delay(5);
        }
        luxValue = luxValue / luxMid;
        snprintf(luxValueBuffer, sizeof(luxValueBuffer), "%.1f", luxValue);
        log_d("Updating Lux: %f", luxValue);
    }
}