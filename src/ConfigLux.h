#ifndef PLANTSENSOR_CONFIGLUX_H
#define PLANTSENSOR_CONFIGLUX_H

#include "Max44009.h"

extern Max44009 SensorLux;
extern float luxValue;
extern char luxValueBuffer[16];
#define luxMid 5

void readLux();

#endif //PLANTSENSOR_CONFIGLUX_H
