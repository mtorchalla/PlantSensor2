#ifndef PLANTSENSOR_CONFIGSCALE_H
#define PLANTSENSOR_CONFIGSCALE_H

#include <Arduino.h>
#include <HX711.h>
#include "Settings.h"

// HX711:
struct Scale {
    HX711* main;
    const char dOut;
    const char sck;
    float offset;
    float scale;
    const char gain;
    char scaleValueBuffer[16];
    const char * mqttUri;
};
#define SCALE_SCK 13
#define SCALE1_DATA 2
#define SCALE2_DATA 2
#define SCALE3_DATA 2
#define SCALE4_DATA 2
#define SCALE5_DATA 2
#define SCALE6_DATA 2
#define SCALE7_DATA 2
#define SCALE8_DATA 2
#define SCALE9_DATA 2
#define SCALE10_DATA 2
HX711 SensorScale1;
HX711 SensorScale2;
HX711 SensorScale3;
HX711 SensorScale4;
HX711 SensorScale5;
HX711 SensorScale6;
HX711 SensorScale7;
HX711 SensorScale8;
Scale Scale1 = { &SensorScale1,  SCALE1_DATA, SCALE_SCK,  -391750 , 23200, 128, "", "" };
Scale Scale2 = { &SensorScale2,  SCALE2_DATA, SCALE_SCK,  -391750 , 23200, 32,  "", "" };
Scale Scale3 = { &SensorScale3,  SCALE3_DATA, SCALE_SCK,  -391750 , 23200, 128, "", "" };
Scale Scale4 = { &SensorScale4,  SCALE4_DATA, SCALE_SCK,  -391750 , 23200, 32,  "", "" };
Scale Scale5 = { &SensorScale5,  SCALE5_DATA, SCALE_SCK,  -391750 , 23200, 128, "", "" };
Scale Scale6 = { &SensorScale6,  SCALE6_DATA, SCALE_SCK,  -391750 , 23200, 32,  "", "" };
Scale Scale7 = { &SensorScale7,  SCALE7_DATA, SCALE_SCK,  -391750 , 23200, 128, "", "" };
Scale Scale8 = { &SensorScale8,  SCALE8_DATA, SCALE_SCK,  -391750 , 23200, 32,  "", "" };
Scale Scale9 = { &SensorScale8,  SCALE9_DATA, SCALE_SCK,  -391750 , 23200, 32,  "", "" };
Scale Scale10 = { &SensorScale8, SCALE10_DATA, SCALE_SCK, -391750 , 23200, 32,  "", "" };
Scale* Scales[10] = {
        &Scale1,
        &Scale2,
        &Scale3,
        &Scale4,
        &Scale5,
        &Scale6,
        &Scale7,
        &Scale8,
        &Scale9,
        &Scale10,
};
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
