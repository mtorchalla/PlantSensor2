#include "WebUi.h"

#define UPDATE_INTERVAL_WEB_UI 3000
#define READ_AVERAGE_SCALE 50


long scaleValueRaw;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
const char *ESPUI_ssid = "ESPUI";
const char *ESPUI_password = "espui";
const char *ESPUI_hostname = "espui";
static bool ui_setup_finished = false;

// Ui-Element Structs:
ScalesTab scalesTab1;
ScalesTab scalesTab2;
ScalesTab scalesTab3;
ScalesTab scalesTab4;
ScalesTab scalesTab5;
ScalesTab scalesTab6;
ScalesTab scalesTab7;
ScalesTab scalesTab8;
ScalesTab scalesTab9;
ScalesTab scalesTab10;
ScalesTab *scalesTabs[NR_OF_SCALES] = {
        &scalesTab1,
        &scalesTab2,
        &scalesTab3,
        &scalesTab4,
        &scalesTab5,
        &scalesTab6,
        &scalesTab7,
        &scalesTab8,
        &scalesTab9,
        &scalesTab10,
};

struct networkTabStruct : BaseUiElements {
    uint16_t textSsid;
    uint16_t textPass;
    uint16_t textMqttServer;
    uint16_t textMqttPort;
    uint16_t textMqttUser;
    uint16_t textMqttPass;
    uint16_t textMqttName;
    uint16_t sliderUpdate;
    uint16_t buttonSave;
} networkTab;

struct batteryTabStruct : BaseUiElements {
    uint16_t batteryMqttUri;
    uint16_t batteryMqttUriLow;
} batteryTab;

struct bmeTabStruct : BaseUiElements {
    uint16_t tempMqttUri;
    uint16_t humMqttUri;
    uint16_t presMqttUri;
} bmeTab;

struct luxTabStruct : BaseUiElements {
    uint16_t luxMqttUri;
} luxTab;

struct valveTabStruct : BaseUiElements {
    uint16_t useValve1 = 0;
    uint16_t useValve2 = 0;
    uint16_t useValve3 = 0;
    uint16_t useValve4 = 0;
    uint16_t useValve5 = 0;
    uint16_t useValve6 = 0;
    uint16_t useValve7 = 0;
    uint16_t useValve8 = 0;
    uint16_t useValve9 = 0;
    uint16_t useValve10 = 0;
    uint16_t voltageValve1 = 0;
    uint16_t voltageValve2 = 0;
    uint16_t voltageValve3 = 0;
    uint16_t voltageValve4 = 0;
    uint16_t voltageValve5 = 0;
    uint16_t voltageValve6 = 0;
    uint16_t voltageValve7 = 0;
    uint16_t voltageValve8 = 0;
    uint16_t voltageValve9 = 0;
    uint16_t voltageValve10 = 0;
    uint16_t selectScale1 = 0;
    uint16_t selectScale2 = 0;
    uint16_t selectScale3 = 0;
    uint16_t selectScale4 = 0;
    uint16_t selectScale5 = 0;
    uint16_t selectScale6 = 0;
    uint16_t selectScale7 = 0;
    uint16_t selectScale8 = 0;
    uint16_t selectScale9 = 0;
    uint16_t selectScale10 = 0;
    uint16_t mqttUriValve1 = 0;
    uint16_t mqttUriValve2 = 0;
    uint16_t mqttUriValve3 = 0;
    uint16_t mqttUriValve4 = 0;
    uint16_t mqttUriValve5 = 0;
    uint16_t mqttUriValve6 = 0;
    uint16_t mqttUriValve7 = 0;
    uint16_t mqttUriValve8 = 0;
    uint16_t mqttUriValve9 = 0;
    uint16_t mqttUriValve10 = 0;
    uint16_t *selectElements[NR_OF_SCALES] = { &selectScale1, &selectScale2, &selectScale3, &selectScale4, &selectScale5, &selectScale6, &selectScale7, &selectScale8, &selectScale9, &selectScale10};
} valveTab;

struct tabsStruct {
    uint16_t tabNetwork;
    uint16_t tabBatteryLevel;
    uint16_t tabBME;
    uint16_t tabLux;
    uint16_t tabValve;
    uint16_t tabScale1;
    uint16_t tabScale2;
    uint16_t tabScale3;
    uint16_t tabScale4;
    uint16_t tabScale5;
    uint16_t tabScale6;
    uint16_t tabScale7;
    uint16_t tabScale8;
    uint16_t tabScale9;
    uint16_t tabScale10;
} tabs;

//typedef struct {
//    ListUiElementWithSettings *listUiElementWithSettings;
//    uint16_t *tabId;
//} UiWithListConnection;

ListUiElementWithSettings tabNetwork = ListUiElementWithSettings(&networkTab);
ListUiElementWithSettings tabBattery = ListUiElementWithSettings(&batteryTab);
ListUiElementWithSettings tabBme     = ListUiElementWithSettings(&bmeTab);
ListUiElementWithSettings tabLux     = ListUiElementWithSettings(&luxTab);
ListUiElementWithSettings tabValve   = ListUiElementWithSettings(&valveTab);
ListUiElementWithSettings tabScale1  = ListUiElementWithSettings(&scalesTab1);
ListUiElementWithSettings tabScale2  = ListUiElementWithSettings(&scalesTab2);
ListUiElementWithSettings tabScale3  = ListUiElementWithSettings(&scalesTab3);
ListUiElementWithSettings tabScale4  = ListUiElementWithSettings(&scalesTab4);
ListUiElementWithSettings tabScale5  = ListUiElementWithSettings(&scalesTab5);
ListUiElementWithSettings tabScale6  = ListUiElementWithSettings(&scalesTab6);
ListUiElementWithSettings tabScale7  = ListUiElementWithSettings(&scalesTab7);
ListUiElementWithSettings tabScale8  = ListUiElementWithSettings(&scalesTab8);
ListUiElementWithSettings tabScale9  = ListUiElementWithSettings(&scalesTab9);
ListUiElementWithSettings tabScale10 = ListUiElementWithSettings(&scalesTab10);
ListUiElementWithSettings* allUiElements[] = {
        &tabNetwork,
        &tabNetwork,
        &tabBattery,
        &tabBme,
        &tabLux,
        &tabValve,
        &tabScale1,
        &tabScale2,
        &tabScale3,
        &tabScale4,
        &tabScale5,
        &tabScale6,
        &tabScale7,
        &tabScale8,
        &tabScale9,
        &tabScale10
};
//UiWithListConnection uiWithListConnection[] { {&tabNetwork, &tabs.tabNetwork},
//                                              {&tabBattery, &tabs.tabBatteryLevel},
//                                              {&tabBme, &tabs.tabBME},
//                                              {&tabLux, &tabs.tabLux},
//                                              {&tabValve, &tabs.tabValve},
//                                              {&tabScale1, &tabs.tabScale1},
//                                              {&tabScale2, &tabs.tabScale2},
//                                              {&tabScale3, &tabs.tabScale3},
//                                              {&tabScale4, &tabs.tabScale4},
//                                              {&tabScale5, &tabs.tabScale5},
//                                              {&tabScale6, &tabs.tabScale6},
//                                              {&tabScale7, &tabs.tabScale7},
//                                              {&tabScale8, &tabs.tabScale8},
//                                              {&tabScale9, &tabs.tabScale9},
//                                              {&tabScale10, &tabs.tabScale10} };

uint16_t activeTab = 0;
uint16_t status_akku = 0;
uint16_t button_save = 0;
uint16_t status_log = 0;
SettingsManager settingsManager;

void saveTab(ListUiElementWithSettings &tab) {
    auto *element = (UiElementWithSettingsBase*)tab.getIterator();
    while (element != nullptr) {
        // save:
        log_d("Setting parameter: %s, element: %s", ESPUI.getControl(element->getUiElement())->value.c_str(), ESPUI.getControl(element->getUiElement())->label);
        element->setEepParameter(ESPUI.getControl(element->getUiElement())->value);
        element->saveEepParameter(settingsManager);

        element = element->next;
    }
}

void updateValue(Control *sender, int type) {
}

void loadTab(ListUiElementWithSettings &tab) {
    auto *element = (UiElementWithSettingsBase*)tab.getIterator();
    while (element != nullptr) {
        // load:
        element->loadEepParameter(settingsManager);
        if (!(ESPUI.getControl(element->getUiElement())->value && element->getEepValue() == 0))  // if not value is empty, String(ESPUI.getControl(element->getUiElement())->label).indexOf("Uri") > 0
            ESPUI.getControl(element->getUiElement())->value = element->getEepValue();
        ESPUI.updateControl(element->getUiElement());

        element = element->next;
    }
}

void saveAndQuit(Control *sender, int type) {
    if (ESPUI.getControl(networkTab.textSsid)->value.isEmpty()) {
        ESPUI.getControl(status_log)->value = "Enter at least a Wifi-SSID!";
        ESPUI.updateControl(status_log);
        return;
    }
    ESPUI.getControl(status_log)->value = "Saving values and rebooting... \nYou may close this tab now.";
    ESPUI.updateControl(status_log);
    for (auto tab : allUiElements)
        saveTab(*tab);
//    saveTab(tabNetwork);
//    saveTab(tabBattery);
//    saveTab(tabBme);
//    saveTab(tabLux);
//    saveTab(tabValve);
//    saveTab(tabScale1);
//    saveTab(tabScale2);
//    saveTab(tabScale3);
//    saveTab(tabScale4);
//    saveTab(tabScale5);
//    saveTab(tabScale6);
//    saveTab(tabScale7);
//    saveTab(tabScale8);
//    saveTab(tabScale9);
//    saveTab(tabScale10);
    ui_setup_finished = true;
}

void saveTabCallback(Control *sender, int type) {
    saveTab(*getListUiElementWithSettingsByControl(sender));
//    for (auto tab : allUiElements) {
//        if (ESPUI.getControl(sender->id)->parentControl == tab->parentTab) {
//            saveTab(*tab);
//            return;
//        }
//    }
};
ListUiElementWithSettings *getListUiElementWithSettingsByControl(Control *control) {
    return getListUiElementWithSettingsByParentId(control->parentControl);
}

ListUiElementWithSettings *getListUiElementWithSettingsByParentId(uint16_t parentId) {
    for (auto tab : allUiElements) {
        if (parentId == tab->parentTab) {
            return tab;
        }
    }
    return nullptr;
}

uint8_t getScaleNumber(Control* sender) {
    if (sender->id ==  scalesTab1.buttonTar || sender->id ==  scalesTab1.buttonCalibrate)  return 1;
    if (sender->id ==  scalesTab2.buttonTar || sender->id ==  scalesTab2.buttonCalibrate)  return 2;
    if (sender->id ==  scalesTab3.buttonTar || sender->id ==  scalesTab3.buttonCalibrate)  return 3;
    if (sender->id ==  scalesTab4.buttonTar || sender->id ==  scalesTab4.buttonCalibrate)  return 4;
    if (sender->id ==  scalesTab5.buttonTar || sender->id ==  scalesTab5.buttonCalibrate)  return 5;
    if (sender->id ==  scalesTab6.buttonTar || sender->id ==  scalesTab6.buttonCalibrate)  return 6;
    if (sender->id ==  scalesTab7.buttonTar || sender->id ==  scalesTab7.buttonCalibrate)  return 7;
    if (sender->id ==  scalesTab8.buttonTar || sender->id ==  scalesTab8.buttonCalibrate)  return 8;
    if (sender->id ==  scalesTab9.buttonTar || sender->id ==  scalesTab9.buttonCalibrate)  return 9;
    if (sender->id == scalesTab10.buttonTar || sender->id ==  scalesTab10.buttonCalibrate) return 10;
    return 0;
}

void tarScale(Control* sender, int type) {
    if (type == B_UP) {
        log_d("Taring Scale:");
        auto scalesTab = (ScalesTab *) getListUiElementWithSettingsByControl(sender)->uiElements;
        ESPUI.updateControlValue(scalesTab->label, "Getting value...");
        log_d("Got scales tab: %i", scalesTab->label);
        EepParameter<float> *scaleOffset = getListUiElementWithSettingsByControl(sender)->getEepParameterFromControl<float>(
                scalesTab->labelTar);
        log_d("Got scaleOffset EEPP: %s, %f", scaleOffset->eepName, scaleOffset->value);
        uint8_t scaleNr = getScaleNumber(sender) - 1;
        log_d("Got Scale Number: %i, %s", scaleNr, Scales[scaleNr]->scaleValueBuffer);
        long value = 0;
        for (int i = 0; i < READ_AVERAGE_SCALE; i++) {
            value += Scales[scaleNr]->main->read();
            delay(0);
        }
        value = value / READ_AVERAGE_SCALE;
        scaleOffset->value = value;  // Scales[scaleNr]->main->read_average(READ_AVERAGE_SCALE);
        log_d("Got Offset Value: %f", scaleOffset->value);
        settingsManager.saveValue(*scaleOffset);
        settingsManager.getConfigScalesAll();  // dont know if necessary
        ESPUI.updateControlValue(scalesTab->labelTar, String(scaleOffset->value));
    //    updateScaleValues();
    }
}

void calibrateScale(Control* sender, int type) {
    if (type == B_UP) {
        log_d("Calibrating Scale:");
        auto scalesTab = (ScalesTab *) getListUiElementWithSettingsByControl(sender)->uiElements;
        ESPUI.updateControlValue(scalesTab->label, "Getting value...");
        log_d("Got scales tab: %i", scalesTab->label);
        EepParameter<float> *scaleScale = getListUiElementWithSettingsByControl(
                sender)->getEepParameterFromControl<float>(scalesTab->labelCalibrate);
        log_d("Got scaleOffset EEPP: %s, %f", scaleScale->eepName, scaleScale->value);
        uint8_t scaleNr = getScaleNumber(sender) - 1;
        log_d("Got Scale Number: %i, %s", scaleNr, Scales[scaleNr]->scaleValueBuffer);
        long value = 0;
        for (int i = 0; i < READ_AVERAGE_SCALE; i++) {
            value += (Scales[scaleNr]->main->read() - Scales[scaleNr]->settings->ScaleOffset.value);
            delay(0);
        }
        value = value / READ_AVERAGE_SCALE;
        scaleScale->value = value;
//    scaleScale->value = (Scales[scaleNr]->main->read_average(READ_AVERAGE_SCALE) - Scales[scaleNr]->settings->ScaleOffset.value);
        log_d("Got Offset Value: %f", scaleScale->value);
        settingsManager.saveValue(*scaleScale);
        settingsManager.getConfigScalesAll();  // dont know if necessary
        ESPUI.updateControlValue(scalesTab->labelCalibrate, String(scaleScale->value));
//    updateScaleValues();
    }
}

void loadSettingsToUi() {
    settingsManager.getConfigScalesAll();
    settingsManager.getConfigBme();
    settingsManager.getConfigLux();
    settingsManager.getConfigBattery();
    settingsManager.getConfigValves();
    for (auto tab : allUiElements)
        loadTab(*tab);
//    loadTab(tabNetwork);
//    loadTab(tabValve);
//    loadTab(tabScale1);
//    loadTab(tabScale2);
//    loadTab(tabScale3);
//    loadTab(tabScale4);
//    loadTab(tabScale5);
//    loadTab(tabScale6);
//    loadTab(tabScale7);
//    loadTab(tabScale8);
//    loadTab(tabScale9);
//    loadTab(tabScale10);
}

void switchTab(Control* sender, int type) {
    activeTab = sender->id;
}

void activateScale(Control* sender, int type) {
    if (sender->id == scalesTab1.switchButton) switchScaleElements(scalesTab1, sender->value);
    if (sender->id == scalesTab2.switchButton) switchScaleElements(scalesTab2, sender->value);
    if (sender->id == scalesTab3.switchButton) switchScaleElements(scalesTab3, sender->value);
    if (sender->id == scalesTab4.switchButton) switchScaleElements(scalesTab4, sender->value);
    if (sender->id == scalesTab5.switchButton) switchScaleElements(scalesTab5, sender->value);
    if (sender->id == scalesTab6.switchButton) switchScaleElements(scalesTab6, sender->value);
    if (sender->id == scalesTab7.switchButton) switchScaleElements(scalesTab7, sender->value);
    if (sender->id == scalesTab8.switchButton) switchScaleElements(scalesTab8, sender->value);
    if (sender->id == scalesTab9.switchButton) switchScaleElements(scalesTab9, sender->value);
    if (sender->id == scalesTab10.switchButton) switchScaleElements(scalesTab10, sender->value);
}

void switchScaleElements(ScalesTab &scalesTab, String value) {
    if (value == "1") {
        ESPUI.getControl(scalesTab.label)->color = ControlColor::Emerald;
        ESPUI.getControl(scalesTab.label)->value = "Getting Scale Value...";
        ESPUI.getControl(scalesTab.buttonTar)->color = ControlColor::Emerald;
        ESPUI.getControl(scalesTab.buttonCalibrate)->color = ControlColor::Emerald;
        ESPUI.getControl(scalesTab.inputMqttUri)->color = ControlColor::Emerald;
        ESPUI.getControl(scalesTab.labelTar)->color = ControlColor::Emerald;
        ESPUI.getControl(scalesTab.labelCalibrate)->color = ControlColor::Emerald;
    }
    else {
        ESPUI.getControl(scalesTab.label)->color = ControlColor::Dark;
        ESPUI.getControl(scalesTab.label)->value = "Not used";
        ESPUI.getControl(scalesTab.buttonTar)->color = ControlColor::Dark;
        ESPUI.getControl(scalesTab.buttonCalibrate)->color = ControlColor::Dark;
        ESPUI.getControl(scalesTab.inputMqttUri)->color = ControlColor::Dark;
        ESPUI.getControl(scalesTab.labelTar)->color = ControlColor::Dark;
        ESPUI.getControl(scalesTab.labelCalibrate)->color = ControlColor::Dark;
    }
    ESPUI.updateControl(scalesTab.label);
    ESPUI.updateControl(scalesTab.buttonTar);
    ESPUI.updateControl(scalesTab.buttonCalibrate);
    ESPUI.updateControl(scalesTab.inputMqttUri);
    ESPUI.updateControl(scalesTab.labelTar);
    ESPUI.updateControl(scalesTab.labelCalibrate);
}

void switchValveElement(uint16_t uiElement1, uint16_t uiElement2, uint16_t uiElement3, String value) {
    if (value == "1") {
        ESPUI.getControl(uiElement1)->color = ControlColor::Emerald;
        ESPUI.getControl(uiElement2)->color = ControlColor::Emerald;
        ESPUI.getControl(uiElement3)->color = ControlColor::Emerald;
    } else {
        ESPUI.getControl(uiElement1)->color = ControlColor::Dark;
        ESPUI.getControl(uiElement2)->color = ControlColor::Dark;
        ESPUI.getControl(uiElement3)->color = ControlColor::Dark;
    }
    ESPUI.updateControl(uiElement1);
    ESPUI.updateControl(uiElement2);
    ESPUI.updateControl(uiElement3);
}

void activateValve(Control* sender, int type) {
    if (sender->id == valveTab.useValve1) switchValveElement(valveTab.voltageValve1, valveTab.selectScale1, valveTab.mqttUriValve1, sender->value);
    if (sender->id == valveTab.useValve2) switchValveElement(valveTab.voltageValve2, valveTab.selectScale2, valveTab.mqttUriValve2, sender->value);
    if (sender->id == valveTab.useValve3) switchValveElement(valveTab.voltageValve3, valveTab.selectScale3, valveTab.mqttUriValve3, sender->value);
    if (sender->id == valveTab.useValve4) switchValveElement(valveTab.voltageValve4, valveTab.selectScale4, valveTab.mqttUriValve4, sender->value);
    if (sender->id == valveTab.useValve5) switchValveElement(valveTab.voltageValve5, valveTab.selectScale5, valveTab.mqttUriValve5, sender->value);
    if (sender->id == valveTab.useValve6) switchValveElement(valveTab.voltageValve6, valveTab.selectScale6, valveTab.mqttUriValve6, sender->value);
    if (sender->id == valveTab.useValve7) switchValveElement(valveTab.voltageValve7, valveTab.selectScale7, valveTab.mqttUriValve7, sender->value);
    if (sender->id == valveTab.useValve8) switchValveElement(valveTab.voltageValve8, valveTab.selectScale8, valveTab.mqttUriValve8, sender->value);
    if (sender->id == valveTab.useValve9) switchValveElement(valveTab.voltageValve9, valveTab.selectScale9, valveTab.mqttUriValve9, sender->value);
    if (sender->id == valveTab.useValve10) switchValveElement(valveTab.voltageValve10, valveTab.selectScale10, valveTab.mqttUriValve10, sender->value);
}

void setup_espui(SettingsManager& newSettingsManager) {
    rtc_wdt_disable();
    settingsManager = newSettingsManager;
    ESPUI.setVerbosity(ESPUI_Verbosity::VerboseJSON);


    WiFi.setHostname(ESPUI_hostname);

    // try to connect to existing network
    WiFi.begin(ESPUI_ssid, ESPUI_password);

    uint8_t timeout = 10;
    // create hot spot
    if (WiFi.status() != WL_CONNECTED) {
        log_d("\n\nCreating Hot Spot");

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
        WiFi.softAP(ESPUI_ssid);

        timeout = 5;

        do {
            delay(500);
            log_d(".");
            timeout--;
        } while (timeout);
    }

    dnsServer.start(DNS_PORT, "*", apIP);

    log_d("WiFi parameters; Mode: %s; IP Address: %s", WiFi.getMode() == WIFI_AP ? "Station" : "Client", WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

    updateBatteryLevel();
    status_akku = ESPUI.addControl(ControlType::Label, "Battery Level:", batteryLevelBuffer, ControlColor::Sunflower);
    status_log = ESPUI.addControl(ControlType::Label, "Status Message", "Please enter your data", ControlColor::Sunflower);
    button_save = ESPUI.addControl(ControlType::Button, "Save and Reboot", "Save", ControlColor::Emerald, Control::noParent, &saveAndQuit);


    setupTabs();
    setupNetworkTab(tabNetwork.parentTab);
    setupBatteryTab(tabBattery.parentTab);
    setupBMETab(tabBme.parentTab);
    setupLuxTab(tabLux.parentTab);
    setupValveTab(tabValve.parentTab);
    setupScalesTab(tabScale1.parentTab, scalesTab1, tabScale1, Settings::settingsScale1, "1");
    setupScalesTab(tabScale2.parentTab, scalesTab2, tabScale2, Settings::settingsScale2, "2");
    setupScalesTab(tabScale3.parentTab, scalesTab3, tabScale3, Settings::settingsScale3, "3");
    setupScalesTab(tabScale4.parentTab, scalesTab4, tabScale4, Settings::settingsScale4, "4");
    setupScalesTab(tabScale5.parentTab, scalesTab5, tabScale5, Settings::settingsScale5, "5");
    setupScalesTab(tabScale6.parentTab, scalesTab6, tabScale6, Settings::settingsScale6, "6");
    setupScalesTab(tabScale7.parentTab, scalesTab7, tabScale7, Settings::settingsScale7, "7");
    setupScalesTab(tabScale8.parentTab, scalesTab8, tabScale8, Settings::settingsScale8, "8");
    setupScalesTab(tabScale9.parentTab, scalesTab9, tabScale9, Settings::settingsScale9, "9");
    setupScalesTab(tabScale10.parentTab, scalesTab10, tabScale10, Settings::settingsScale10, "10");

    // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
    // ESPUI.sliderContinuous = true;
    ESPUI.jsonUpdateDocumentSize = 3500; // 2000 is the default, and this value is not affected by the amount of widgets
    ESPUI.jsonInitialDocumentSize = 40000; // 8000 is the default, adjust when you have too many widgets or options
    ESPUI.begin("Smart Garden Config"); // , "username", "password")
    loop_espui();
}

void updateBatteryLevel() {
    readBatteryLevel(5);
    ESPUI.updateControlValue(status_akku, (String)getBatteryLevel() + "%");
}

void setupTabs() {
    tabNetwork.parentTab = ESPUI.addControl( ControlType::Tab, "Network", "Network", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabBattery.parentTab = ESPUI.addControl( ControlType::Tab, "Battery", "Battery", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabBme.parentTab = ESPUI.addControl( ControlType::Tab, "Environment Sensor", "Environment Sensor", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabLux.parentTab = ESPUI.addControl( ControlType::Tab, "Light Sensor", "Light Sensor", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabValve.parentTab = ESPUI.addControl( ControlType::Tab, "Valves/Motors", "Valves", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabScale1.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 1", "Scale 1", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale2.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 2", "Scale 2", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale3.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 3", "Scale 3", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale4.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 4", "Scale 4", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale5.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 5", "Scale 5", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale6.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 6", "Scale 6", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale7.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 7", "Scale 7", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale8.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 8", "Scale 8", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale9.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 9", "Scale 9", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabScale10.parentTab = ESPUI.addControl( ControlType::Tab, "Scale 10", "Scale 10", ControlColor::Alizarin,  Control::noParent, &switchTab);
}

void setupNetworkTab(uint16_t parentTab) {
    networkTab.textSsid = ESPUI.addControl(ControlType::Text, "Network SSID", "", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<String>(networkTab.textSsid, &Settings::settingsNetwork.WifiSSid);
    networkTab.textPass = ESPUI.addControl(ControlType::Text, "Network Password", "", ControlColor::Peterriver, parentTab, &updateValue) ;
    tabNetwork.addElement<String>(networkTab.textPass, &Settings::settingsNetwork.WifiPass);
    networkTab.textMqttServer = ESPUI.addControl(ControlType::Text, "MQTT Server Address", "", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<String>(networkTab.textMqttServer, &Settings::settingsNetwork.MqttServer);
    networkTab.textMqttPort = ESPUI.addControl(ControlType::Text, "MQTT Server Port", "1883", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<uint16_t>(networkTab.textMqttPort, &Settings::settingsNetwork.MqttPort);
    networkTab.textMqttUser = ESPUI.addControl(ControlType::Text, "MQTT User Name", "", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<String>(networkTab.textMqttUser, &Settings::settingsNetwork.MqttUser);
    networkTab.textMqttPass = ESPUI.addControl(ControlType::Text, "MQTT Password", "", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<String>(networkTab.textMqttPass, &Settings::settingsNetwork.MqttPass);
    networkTab.textMqttName = ESPUI.addControl(ControlType::Text, "MQTT Client Name", "", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<String>(networkTab.textMqttName, &Settings::settingsNetwork.MqttName);
    networkTab.sliderUpdate = ESPUI.addControl(ControlType::Slider, "Update Interval in Minutes (Smaller Values = more Power Consumption)", "10", ControlColor::Peterriver, parentTab, &updateValue);
    tabNetwork.addElement<uint8_t>(networkTab.sliderUpdate, &Settings::settingsNetwork.UpdateInterval);
    networkTab.buttonSave = ESPUI.addControl(ControlType::Button, "Save", "Save",ControlColor::Wetasphalt, parentTab, &saveTabCallback);
}

void setupScalesTab(uint16_t parentTab, ScalesTab &scalesTab, ListUiElementWithSettings &tabScale, EepStructScale &eepStructScale, String scaleNumber) {
    log_d("Setting up Scales Tab %s...", scaleNumber.c_str());
    scalesTab.switchButton = ESPUI.addControl(ControlType::Switcher, "Use Scale", "0", ControlColor::Emerald, parentTab, &activateScale);
    tabScale.addElement<bool>(scalesTab.switchButton, &eepStructScale.ScaleInUse);
    scalesTab.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri", String("/home/smart_garden/scale") + scaleNumber, ControlColor::Emerald, parentTab, &updateValue);
    tabScale.addElement<String>(scalesTab.inputMqttUri, &eepStructScale.ScaleMqttUri);
    scalesTab.label = ESPUI.addControl(ControlType::Label, "Current Value in kg", "", ControlColor::Emerald, parentTab);
    scalesTab.buttonTar = ESPUI.addControl(ControlType::Button, "Tar Scale", "Tar", ControlColor::Emerald, parentTab, &tarScale);
    scalesTab.buttonCalibrate = ESPUI.addControl(ControlType::Button, "Calibrate: Set current Value to 1 kg", "Calibrate", ControlColor::Emerald, parentTab, &calibrateScale);
    scalesTab.labelTar = ESPUI.addControl(ControlType::Label, "Tar Value", "", ControlColor::Emerald, parentTab);
    tabScale.addElement<float>(scalesTab.labelTar, &eepStructScale.ScaleOffset);
    scalesTab.labelCalibrate = ESPUI.addControl(ControlType::Label, "Scaling Value", "", ControlColor::Emerald, parentTab);
    tabScale.addElement<float>(scalesTab.labelCalibrate, &eepStructScale.ScaleScale);
    scalesTab.buttonSave = ESPUI.addControl(ControlType::Button, "Save Scale Settings", "Save", ControlColor::Wetasphalt, parentTab, &saveTabCallback);
}

void set_switches() {
    activateScale(ESPUI.getControl(scalesTab1.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab2.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab3.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab4.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab5.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab6.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab7.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab8.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab9.switchButton), 0);
    activateScale(ESPUI.getControl(scalesTab10.switchButton), 0);
    activateValve(ESPUI.getControl(valveTab.useValve1), 0);
    activateValve(ESPUI.getControl(valveTab.useValve2), 0);
    activateValve(ESPUI.getControl(valveTab.useValve3), 0);
    activateValve(ESPUI.getControl(valveTab.useValve4), 0);
    activateValve(ESPUI.getControl(valveTab.useValve5), 0);
    activateValve(ESPUI.getControl(valveTab.useValve6), 0);
    activateValve(ESPUI.getControl(valveTab.useValve7), 0);
    activateValve(ESPUI.getControl(valveTab.useValve8), 0);
    activateValve(ESPUI.getControl(valveTab.useValve9), 0);
    activateValve(ESPUI.getControl(valveTab.useValve10), 0);
}

void updateScaleValues_(ScalesTab &scaleT, Scale &scale) {
//    log_d("Updating Scale %s; Value: %s", scale.settings->ScaleMqttUri.value.c_str(), scale.scaleValueBuffer);
    ESPUI.getControl(scaleT.label)->value = scale.scaleValueBuffer;
    ESPUI.updateControl(scaleT.label);
}

void updateScaleValues() {
    ::readScales();
    for (int i=0; i < NR_OF_SCALES; i++) {
        if (Scales[i]->settings->ScaleInUse.value) {
            updateScaleValues_(*scalesTabs[i], *Scales[i]);
        }
    }
}

void loop_espui() {
    loadSettingsToUi();
    set_switches();
    while (!ui_setup_finished) {
        dnsServer.processNextRequest();

        static long oldTime = 0;

        if (millis() - oldTime > UPDATE_INTERVAL_WEB_UI) {
            updateBatteryLevel();
            updateScaleValues();
            oldTime = millis();
        }
    }
    ESP.restart();
    delay(10000);
}

void setupBatteryTab(uint16_t parentTab) {
    batteryTab.batteryMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Battery Uri", "/home/smart_garden/batteryLevel/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBattery.addElement<String>(batteryTab.batteryMqttUri, &Settings::settingsBattery.MqttUri);
    batteryTab.batteryMqttUriLow = ESPUI.addControl(ControlType::Text, "MQTT Battery Uri Warning", "/home/smart_garden/batteryLow/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBattery.addElement<String>(batteryTab.batteryMqttUriLow, &Settings::settingsBattery.MqttUriLow);
}

void setupBMETab(uint16_t parentTab) {
    bmeTab.tempMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Temperature Uri", "/home/smart_garden/temp/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBme.addElement<String>(bmeTab.tempMqttUri, &Settings::settingsBme.MqttUriTemp);
    bmeTab.humMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Humidity Uri", "/home/smart_garden/hum/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBme.addElement<String>(bmeTab.humMqttUri, &Settings::settingsBme.MqttUriHum);
    bmeTab.presMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Pressure Uri", "/home/smart_garden/pres/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBme.addElement<String>(bmeTab.presMqttUri, &Settings::settingsBme.MqttUriPres);
}

void setupLuxTab(uint16_t parentTab) {
    luxTab.luxMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Light Level Uri", "/home/smart_garden/lux/", ControlColor::Peterriver, parentTab, &updateValue);
    tabLux.addElement<String>(luxTab.luxMqttUri, &Settings::settingsLux.MqttUri);
}

void selectScale(Control *sender, int type) {

}

void setupValveTab(uint16_t parentTab) {
    // Valve 1
    valveTab.useValve1 = ESPUI.addControl(ControlType::Switcher, "Use Output 1", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve1, &Settings::settingsValves.useValve1);
    valveTab.voltageValve1 = ESPUI.addControl(ControlType::Text, "Output 1 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve1, &Settings::settingsValves.voltageValve1);
    valveTab.selectScale1 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale1, &Settings::settingsValves.selectScale1);
    valveTab.mqttUriValve1 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 1:", "/home/smart_garden/actuator1", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve1, &Settings::settingsValves.mqttUriValve1);
    // Valve 2
    valveTab.useValve2 = ESPUI.addControl(ControlType::Switcher, "Use Output 2", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve2, &Settings::settingsValves.useValve2);
    valveTab.voltageValve2 = ESPUI.addControl(ControlType::Text, "Output 2 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve2, &Settings::settingsValves.voltageValve2);
    valveTab.selectScale2 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale2, &Settings::settingsValves.selectScale2);
    valveTab.mqttUriValve2 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 2:", "/home/smart_garden/actuator2", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve2, &Settings::settingsValves.mqttUriValve2);
    // Valve 3
    valveTab.useValve3 = ESPUI.addControl(ControlType::Switcher, "Use Output 3", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve3, &Settings::settingsValves.useValve3);
    valveTab.voltageValve3 = ESPUI.addControl(ControlType::Text, "Output 3 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve3, &Settings::settingsValves.voltageValve3);
    valveTab.selectScale3 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale3, &Settings::settingsValves.selectScale3);
    valveTab.mqttUriValve3 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 3:", "/home/smart_garden/actuator3", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve3, &Settings::settingsValves.mqttUriValve3);
    // Valve 4
    valveTab.useValve4 = ESPUI.addControl(ControlType::Switcher, "Use Output 4", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve4, &Settings::settingsValves.useValve4);
    valveTab.voltageValve4 = ESPUI.addControl(ControlType::Text, "Output 4 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve4, &Settings::settingsValves.voltageValve4);
    valveTab.selectScale4 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale4, &Settings::settingsValves.selectScale4);
    valveTab.mqttUriValve4 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 4:", "/home/smart_garden/actuator4", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve4, &Settings::settingsValves.mqttUriValve4);
    // Valve 5
    valveTab.useValve5 = ESPUI.addControl(ControlType::Switcher, "Use Output 5", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve5, &Settings::settingsValves.useValve5);
    valveTab.voltageValve5 = ESPUI.addControl(ControlType::Text, "Output 5 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve5, &Settings::settingsValves.voltageValve5);
    valveTab.selectScale5 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale5, &Settings::settingsValves.selectScale5);
    valveTab.mqttUriValve5 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 5:", "/home/smart_garden/actuator5", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve5, &Settings::settingsValves.mqttUriValve5);
    // Valve 6
    valveTab.useValve6 = ESPUI.addControl(ControlType::Switcher, "Use Output 6", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve6, &Settings::settingsValves.useValve6);
    valveTab.voltageValve6 = ESPUI.addControl(ControlType::Text, "Output 6 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve6, &Settings::settingsValves.voltageValve6);
    valveTab.selectScale6 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale6, &Settings::settingsValves.selectScale6);
    valveTab.mqttUriValve6 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 6:", "/home/smart_garden/actuator6", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve6, &Settings::settingsValves.mqttUriValve6);
    // Valve 7
    valveTab.useValve7 = ESPUI.addControl(ControlType::Switcher, "Use Output 7", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve7, &Settings::settingsValves.useValve7);
    valveTab.voltageValve7 = ESPUI.addControl(ControlType::Text, "Output 7 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve7, &Settings::settingsValves.voltageValve7);
    valveTab.selectScale7 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale7, &Settings::settingsValves.selectScale7);
    valveTab.mqttUriValve7 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 7:", "/home/smart_garden/actuator7", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve7, &Settings::settingsValves.mqttUriValve7);
    // Valve 8
    valveTab.useValve8 = ESPUI.addControl(ControlType::Switcher, "Use Output 8", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve8, &Settings::settingsValves.useValve8);
    valveTab.voltageValve8 = ESPUI.addControl(ControlType::Text, "Output 8 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve8, &Settings::settingsValves.voltageValve8);
    valveTab.selectScale8 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale8, &Settings::settingsValves.selectScale8);
    valveTab.mqttUriValve8 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 8:", "/home/smart_garden/actuator8", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve8, &Settings::settingsValves.mqttUriValve8);
    // Valve 9
    valveTab.useValve9 = ESPUI.addControl(ControlType::Switcher, "Use Output 9", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve9, &Settings::settingsValves.useValve9);
    valveTab.voltageValve9 = ESPUI.addControl(ControlType::Text, "Output 9 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve9, &Settings::settingsValves.voltageValve9);
    valveTab.selectScale9 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale9, &Settings::settingsValves.selectScale9);
    valveTab.mqttUriValve9 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 9:", "/home/smart_garden/actuator9", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve9, &Settings::settingsValves.mqttUriValve9);
    // Valve 10
    valveTab.useValve10 = ESPUI.addControl(ControlType::Switcher, "Use Output 10", "0", ControlColor::Emerald, parentTab, &activateValve);
    tabValve.addElement<bool>(valveTab.useValve10, &Settings::settingsValves.useValve10);
    valveTab.voltageValve10 = ESPUI.addControl(ControlType::Text, "Output 10 Voltage", "5.0", ControlColor::Emerald, parentTab, &updateValue);
    tabValve.addElement<float>(valveTab.voltageValve10, &Settings::settingsValves.voltageValve10);
    valveTab.selectScale10 = ESPUI.addControl( ControlType::Select, "Use with Scale:", "", ControlColor::Emerald, parentTab, &selectScale );
    tabValve.addElement<uint8_t>(valveTab.selectScale10, &Settings::settingsValves.selectScale10);
    valveTab.mqttUriValve10 = ESPUI.addControl( ControlType::Text, "MQTT Uri Valve 10:", "/home/smart_garden/actuator10", ControlColor::Emerald, parentTab, &updateValue );
    tabValve.addElement<String>(valveTab.mqttUriValve10, &Settings::settingsValves.mqttUriValve10);

    for (int i=0; i<NR_OF_SCALES; i++) {
        ESPUI.addControl( ControlType::Option, "Scale Sensor 1", "Scale1", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 2", "Scale2", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 3", "Scale3", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 4", "Scale4", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 5", "Scale5", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 6", "Scale6", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 7", "Scale7", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 8", "Scale8", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 9", "Scale9", ControlColor::Alizarin, *valveTab.selectElements[i] );
        ESPUI.addControl( ControlType::Option, "Scale Sensor 10", "Scale10", ControlColor::Alizarin, *valveTab.selectElements[i] );
    }

}