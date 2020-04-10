#ifndef PLANTSENSOR_EEPPARAMETERTYPES_H
#define PLANTSENSOR_EEPPARAMETERTYPES_H

#include <Arduino.h>

template<typename T>
struct EepParameter {
    T value;
    const char *eepName;
};

#define NR_OF_SCALES 10
typedef struct EepStructScale {
    EepParameter<float> ScaleOffset;
    EepParameter<float> ScaleScale;
    EepParameter<String> ScaleMqttUri;
    EepParameter<bool> ScaleInUse;
} EepStructScale;

#endif //PLANTSENSOR_EEPPARAMETERTYPES_H
