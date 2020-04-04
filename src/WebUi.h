#ifndef PLANTSENSOR_WEBUI_H
#define PLANTSENSOR_WEBUI_H
#include <Arduino.h>
#include <ESPUI.h>
#include <DNSServer.h>
#include "SettingsManager.h"
#include <nvs_flash.h>
#include "SettingsManager.h"
#include "PlantSensor.h"
#include "ConfigBattery.h"
#include <WiFi.h>
#include "ListUiElementWithSettings.h"

class ListUiElementWithSettings;

extern uint16_t status_akku;
extern uint16_t button_save;
extern uint16_t activeTab;


typedef struct {
    uint16_t switchButton;
    uint16_t label;
    uint16_t buttonTar;
    uint16_t buttonCalibrate;
    uint16_t inputMqttUri;
    uint16_t buttonSave;
    uint16_t labelTar;
    uint16_t labelCalibrate;
} ScalesTab;

//typedef struct {
//    uint16_t switchScale;
//    uint16_t labelScale;
//    uint16_t buttonTarScale;
//    uint16_t buttonCalibrateScale;
//    uint16_t inputMqttUri;
//    float calculatedScale;
//    float calculatedOffset;
//} scaleSettings;

void loadSettingsToUi();
void saveAndQuit(Control *sender, int type);
void saveTabCallback(Control* sender, int type);
void updateBatteryLevel();
void setup_espui(SettingsManager& newSettingsManager);
void loop_espui();
void setupTabs();
void setupNetworkTab(uint16_t networkTab);
void setupBatteryTab(uint16_t parentTab);
void setupBMETab(uint16_t parentTab);
void setupLuxTab(uint16_t parentTab);
void setupScalesTab(uint16_t parentTab, ScalesTab &scalesTab, ListUiElementWithSettings &tabScale, EepStructScale &eepStructScale, String scaleNumber);
void activateScale(Control *sender, int type);
void switchScaleElements(ScalesTab &scalesTab, String value);
void switchTab(Control* sender, int type);
//void saveTab(ListUiElementWithSettings &tab);


#endif //PLANTSENSOR_WEBUI_H
