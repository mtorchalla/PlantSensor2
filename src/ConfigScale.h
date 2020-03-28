#ifndef PLANTSENSOR_CONFIGSCALE_H
#define PLANTSENSOR_CONFIGSCALE_H

#include <Arduino.h>
#include <HX711.h>
#include "Settings.h"

// HX711:
struct Scale {
    HX711* main;
    const char dout;
    const char sck;
    const float offset;
    const float scale;
    const char gain;
    char cscale[16];
    char const * mqttUri;
};
#define ScaleSCK 13
HX711 SensorScale1;
HX711 SensorScale2;
HX711 SensorScale3;
HX711 SensorScale4;
HX711 SensorScale5;
HX711 SensorScale6;
HX711 SensorScale7;
HX711 SensorScale8;
Scale Scale1 = { &SensorScale1,  2, ScaleSCK, -391750 , 23200, 128, "",  MQTT_TOPIC_PLANT1 };
Scale Scale2 = { &SensorScale2,  2, ScaleSCK, -391750 , 23200, 32, "", MQTT_TOPIC_PLANT2 };
Scale Scale3 = { &SensorScale3, 12, ScaleSCK, -391750 , 23200, 128, "",  MQTT_TOPIC_PLANT3 };
Scale Scale4 = { &SensorScale4, 12, ScaleSCK, -391750 , 23200, 32, "", MQTT_TOPIC_PLANT4 };
Scale Scale5 = { &SensorScale5, 10, ScaleSCK, -391750 , 23200, 128, "",  MQTT_TOPIC_PLANT5 };
Scale Scale6 = { &SensorScale6, 10, ScaleSCK, -391750 , 23200, 32, "", MQTT_TOPIC_PLANT6 };
Scale Scale7 = { &SensorScale7, 14, ScaleSCK, -391750 , 23200, 128, "",  MQTT_TOPIC_PLANT7 };
Scale Scale8 = { &SensorScale8, 14, ScaleSCK, -391750 , 23200, 32, "", MQTT_TOPIC_PLANT8 };
#ifndef NrOfScales
#elif   NrOfScales == 1
Scale* Scales[NrOfScales] = { &Scale1 };
#elif   NrOfScales == 2
Scale* Scales[NrOfScales] = { &Scale1, &Scale2 };
#elif   NrOfScales == 3
Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3 };
#elif   NrOfScales == 4
			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4 };
#elif   NrOfScales == 5
			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5 };
#elif   NrOfScales == 6
			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5, &Scale6 };
#elif   NrOfScales == 7
			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5, &Scale6, &Scale7 };
#elif   NrOfScales == 8
			Scale* Scales[NrOfScales] = { &Scale1, &Scale2, &Scale3, &Scale4, &Scale5, &Scale6, &Scale7, &Scale8 };
#else
			#error "Unexpected value of NrOfScales."
#endif


#endif //PLANTSENSOR_CONFIGSCALE_H
