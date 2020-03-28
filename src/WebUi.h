#ifndef PLANTSENSOR_WEBUI_H
#define PLANTSENSOR_WEBUI_H
#include <Arduino.h>
#include <ESPUI.h>
#include "SettingsManager.h"

extern uint16_t status_akku;
extern uint16_t button_save;
extern uint16_t activeTab;

typedef struct {
    uint16_t switchScale;
    uint16_t labelScale;
    uint16_t buttonTarScale;
    uint16_t buttonCalibrateScale;
    uint16_t inputMqttUri;
    float calculatedScale;
    float calculatedOffset;
} scaleSettings;

void loadSettingsToUi();
void saveAndQuit(Control *sender, int type);
void saveTabNetwork(Control *sender, int type);
void saveTabScales(Control *sender, int type);
void loadScaleSettingsAll();
void loadNetworkSettings();
void loadScaleSettings(scaleSettings &scaleUiStruct, EepStructScale &eepStructScale);
void saveScaleToSettings(scaleSettings &scaleUiStruct, EepStructScale &eepStructScale);
void update_akku();
void setup_espui(SettingsManager& newSettingsManager);
void loop_espui();
void textCallMy(Control *sender, int type);
void sliderMy(Control *sender, int type);
void buttonCallbackMy(Control *sender, int type);
void buttonExampleMy(Control *sender, int type);
void setupTabs();
void setupNetworkTab(uint16_t networkTab);
void setupScalesTab(uint16_t scalesTab);
void activateScale(Control *sender, int type);
void switchScaleElements(scaleSettings &scaleStruct, String value);
void switchTab(Control* sender, int type);

#endif //PLANTSENSOR_WEBUI_H
