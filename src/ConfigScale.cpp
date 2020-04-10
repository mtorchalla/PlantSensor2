#include "ConfigScale.h"

HX711 SensorScale1;
HX711 SensorScale2;
HX711 SensorScale3;
HX711 SensorScale4;
HX711 SensorScale5;
HX711 SensorScale6;
HX711 SensorScale7;
HX711 SensorScale8;
HX711 SensorScale9;
HX711 SensorScale10;
Scale Scale1  = { &SensorScale1,  &Settings::settingsScale1,  SCALE1_DATA,  SCALE_SCK, 128, "" };
Scale Scale2  = { &SensorScale2,  &Settings::settingsScale2,  SCALE2_DATA,  SCALE_SCK, 32,  "" };
Scale Scale3  = { &SensorScale3,  &Settings::settingsScale3,  SCALE3_DATA,  SCALE_SCK, 128, "" };
Scale Scale4  = { &SensorScale4,  &Settings::settingsScale4,  SCALE4_DATA,  SCALE_SCK, 32,  "" };
Scale Scale5  = { &SensorScale5,  &Settings::settingsScale5,  SCALE5_DATA,  SCALE_SCK, 128, "" };
Scale Scale6  = { &SensorScale6,  &Settings::settingsScale6,  SCALE6_DATA,  SCALE_SCK, 32,  "" };
Scale Scale7  = { &SensorScale7,  &Settings::settingsScale7,  SCALE7_DATA,  SCALE_SCK, 128, "" };
Scale Scale8  = { &SensorScale8,  &Settings::settingsScale8,  SCALE8_DATA,  SCALE_SCK, 32,  "" };
Scale Scale9  = { &SensorScale9,  &Settings::settingsScale9,  SCALE9_DATA,  SCALE_SCK, 128, "" };
Scale Scale10 = { &SensorScale10, &Settings::settingsScale10, SCALE10_DATA, SCALE_SCK, 32,  "" };

Scale* Scales[NR_OF_SCALES] = {
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

void readScales() {
    log_d("Begin reading scales...");
    for (int i = 0; i < NR_OF_SCALES; i++) {
        if (Scales[i]->settings->ScaleInUse.value) {
            log_d("Try reading scale: %s, SCK: %i, DOUT: %i, Offset: %f, Scale: %f, Gain: %i", Scales[i]->settings->ScaleMqttUri.value.c_str(), Scales[i]->sck, Scales[i]->dOut, Scales[i]->settings->ScaleOffset.value, Scales[i]->settings->ScaleScale.value, Scales[i]->gain);
            Scales[i]->main->begin(Scales[i]->dOut, Scales[i]->sck);
            Scales[i]->main->set_gain(Scales[i]->gain);
            scaleValueRaw = Scales[i]->main->read_average(10);
            log_d("Read Raw Value: %f", scaleValue);
            scaleValue = (scaleValueRaw - Scales[i]->settings->ScaleOffset.value) / Scales[i]->settings->ScaleScale.value;
            // scaleValue = (scaleValue + 391750) / 23200;
            snprintf(Scales[i]->scaleValueBuffer, sizeof(Scales[i]->scaleValueBuffer), "%.2f", scaleValue);
            log_d("Scale Nr.: %i; Value: %f", i + 1, scaleValue);
        }
    }
    log_d("end reading scales...");
//    digitalWrite(SCALE_SCK, HIGH);	// set scales to sleep
}
