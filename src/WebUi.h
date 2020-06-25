#ifndef PLANTSENSOR_WEBUI_H
#define PLANTSENSOR_WEBUI_H
#include <Arduino.h>
#include <ESPUI.h>
#include <DNSServer.h>
#include <nvs_flash.h>
#include "SettingsManager.h"
#include "PlantSensor.h"
#include "ConfigBattery.h"
#include "ConfigScale.h"
#include "SettingsManager.h"
#include <WiFi.h>
#include "ListUiElementWithSettings.h"
#include "ConfigSensorTemp.h"
#include "ConfigLux.h"
#include <soc/rtc_wdt.h>

class ListUiElementWithSettings;

extern uint16_t status_akku;
extern uint16_t button_save;
extern uint16_t activeTab;


typedef struct : BaseUiElements {
    uint16_t switchButton;
    uint16_t label;
    uint16_t buttonTar;
    uint16_t buttonCalibrate;
    uint16_t inputMqttUri;
    uint16_t buttonSave;
    uint16_t labelTar;
    uint16_t labelCalibrate;
    uint16_t noOfMembers = 8;
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
void tarScale(Control* sender, int type);
void calibrateScale(Control* sender, int type);
void selectScale(Control *sender, int type);
ListUiElementWithSettings *getListUiElementWithSettingsByParentId(uint16_t parentId);
ListUiElementWithSettings *getListUiElementWithSettingsByControl(Control *control);
void updateBatteryLevel();
void setup_espui(SettingsManager& newSettingsManager);
void loop_espui();
void setSwitches();
void setupTabs();
void updateScaleValues_(ScalesTab &scaleT, Scale &scale);
void updateScaleValues();
void setupNetworkTab(uint16_t networkTab);
void setupBatteryTab(uint16_t parentTab);
void setupBMETab(uint16_t parentTab);
void setupLuxTab(uint16_t parentTab);
void setupValveTab(uint16_t parentTab);
void activateValve(Control* sender, int type);
void switchValveElement(uint16_t uiElement1, uint16_t uiElement2, uint16_t uiElement3, String value);
void setupScalesTab(uint16_t parentTab, ScalesTab &scalesTab, ListUiElementWithSettings &tabScale, EepStructScale &eepStructScale, String scaleNumber);
void activateScale(Control *sender, int type);
void switchScaleElements(ScalesTab &scalesTab, String value);
void switchTab(Control* sender, int type);
//void saveTab(ListUiElementWithSettings &tab);


#endif //PLANTSENSOR_WEBUI_H
