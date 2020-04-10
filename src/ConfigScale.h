#ifndef PLANTSENSOR_CONFIGSCALE_H
#define PLANTSENSOR_CONFIGSCALE_H

#include <Arduino.h>
#include <HX711.h>
#include "Settings.h"
#include "EepParameterTypes.h"
#include "SettingsManager.h"

// HX711:
struct Scale {
    HX711* main;
    EepStructScale *settings;
    const char dOut;
    const char sck;
//    float offset;
//    float scale;
    const char gain;
    char scaleValueBuffer[16];
//    const char * mqttUri;

};
#define SCALE_SCK 27
#define SCALE1_DATA 26
#define SCALE2_DATA 2
#define SCALE3_DATA 2
#define SCALE4_DATA 2
#define SCALE5_DATA 2
#define SCALE6_DATA 2
#define SCALE7_DATA 2
#define SCALE8_DATA 2
#define SCALE9_DATA 2
#define SCALE10_DATA 2
//typedef float double;
extern Scale Scale1;
extern Scale Scale2;
extern Scale Scale3;
extern Scale Scale4;
extern Scale Scale5;
extern Scale Scale6;
extern Scale Scale7;
extern Scale Scale8;
extern Scale Scale9;
extern Scale Scale10;
extern Scale* Scales[NR_OF_SCALES];

extern double scaleValue;
extern long scaleValueRaw;
extern void readScales();
//#ifndef NrOfScales
//#elif   NrOfScales == 1
//Scale* Scales[NrOfScales] = { &Scale1 };
//#elif   NrOfScales == 2
//Scale* Scales[NrOfScales] = { &Scale1, &Scale2 };
//#elif   NrOfScales == 3
//Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3 };
//#elif   NrOfScales == 4
//			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4 };
//#elif   NrOfScales == 5
//			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5 };
//#elif   NrOfScales == 6
//			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5, &Scale6 };
//#elif   NrOfScales == 7
//			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5, &Scale6, &Scale7 };
//#elif   NrOfScales == 8
//			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5, &Scale6, &Scale7, &Scale8 };
//#else
//			#error "Unexpected value of NrOfScales."
//#endif


#endif //PLANTSENSOR_CONFIGSCALE_H
