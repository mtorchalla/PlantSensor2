#line 2

#include "WebUi.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
const char *ESPUI_ssid = "ESPUI";
const char *ESPUI_password = "espui";
const char *ESPUI_hostname = "espui";
static bool ui_setup_finished = false;

struct tabsStruct {
    uint16_t tabNetwork;
    uint16_t tabBatteryLevel;
    uint16_t tabBME;
    uint16_t tabLux;
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

typedef struct {
    ListUiElementWithSettings *listUiElementWithSettings;
    uint16_t *tabId;
} UiWithListConnection;

ListUiElementWithSettings tabNetwork = ListUiElementWithSettings();
ListUiElementWithSettings tabBattery = ListUiElementWithSettings();
ListUiElementWithSettings tabBme = ListUiElementWithSettings();
ListUiElementWithSettings tabLux = ListUiElementWithSettings();
ListUiElementWithSettings tabScale1 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale2 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale3 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale4 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale5 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale6 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale7 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale8 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale9 = ListUiElementWithSettings();
ListUiElementWithSettings tabScale10 = ListUiElementWithSettings();
UiWithListConnection uiWithListConnection[] { {&tabNetwork, &tabs.tabNetwork},
                                              {&tabBattery, &tabs.tabBatteryLevel},
                                              {&tabBme, &tabs.tabBME},
                                              {&tabLux, &tabs.tabLux},
                                              {&tabScale1, &tabs.tabScale1},
                                              {&tabScale2, &tabs.tabScale2},
                                              {&tabScale3, &tabs.tabScale3},
                                              {&tabScale4, &tabs.tabScale4},
                                              {&tabScale5, &tabs.tabScale5},
                                              {&tabScale6, &tabs.tabScale6},
                                              {&tabScale7, &tabs.tabScale7},
                                              {&tabScale8, &tabs.tabScale8},
                                              {&tabScale9, &tabs.tabScale9},
                                              {&tabScale10, &tabs.tabScale10} };

uint16_t activeTab = 0;
uint16_t status_akku = 0;
uint16_t button_save = 0;
uint16_t status_log = 0;
SettingsManager settingsManager;

struct networkTabStruct {
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

struct batteryTabStruct {
    uint16_t batteryMqttUri;
    uint16_t batteryMqttUriLow;
} batteryTab;

struct bmeTabStruct {
    uint16_t tempMqttUri;
    uint16_t humMqttUri;
    uint16_t presMqttUri;
} bmeTab;

struct luxTabStruct {
    uint16_t luxMqttUri;
} luxTab;

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

uint16_t button1;
uint16_t millisLabelId;
uint16_t switchOne;

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
    saveTab(tabNetwork);
    saveTab(tabBattery);
    saveTab(tabBme);
    saveTab(tabLux);
    saveTab(tabScale1);
    saveTab(tabScale2);
    saveTab(tabScale3);
    saveTab(tabScale4);
    saveTab(tabScale5);
    saveTab(tabScale6);
    saveTab(tabScale7);
    saveTab(tabScale8);
    saveTab(tabScale9);
    saveTab(tabScale10);
    ui_setup_finished = true;
}

void saveTabCallback(Control *sender, int type) {
    for (auto list : uiWithListConnection) {
        if (ESPUI.getControl(sender->id)->parentControl == *list.tabId) {
            saveTab(*list.listUiElementWithSettings);
            return;
        }
    }
};


void loadSettingsToUi() {
    loadTab(tabNetwork);
    loadTab(tabScale1);
    loadTab(tabScale2);
    loadTab(tabScale3);
    loadTab(tabScale4);
    loadTab(tabScale5);
    loadTab(tabScale6);
    loadTab(tabScale7);
    loadTab(tabScale8);
    loadTab(tabScale9);
    loadTab(tabScale10);
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

void setup_espui(SettingsManager& newSettingsManager) {
    settingsManager = newSettingsManager;
    ESPUI.setVerbosity(ESPUI_Verbosity::Quiet);


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
    setupNetworkTab(tabs.tabNetwork);
    setupBatteryTab(tabs.tabBatteryLevel);
    setupBMETab(tabs.tabBME);
    setupLuxTab(tabs.tabLux);
    setupScalesTab(tabs.tabScale1, scalesTab1, tabScale1, Settings::settingsScale1, "1");
    setupScalesTab(tabs.tabScale2, scalesTab2, tabScale2, Settings::settingsScale2, "2");
    setupScalesTab(tabs.tabScale3, scalesTab3, tabScale3, Settings::settingsScale3, "3");
    setupScalesTab(tabs.tabScale4, scalesTab4, tabScale4, Settings::settingsScale4, "4");
    setupScalesTab(tabs.tabScale5, scalesTab5, tabScale5, Settings::settingsScale5, "5");
    setupScalesTab(tabs.tabScale6, scalesTab6, tabScale6, Settings::settingsScale6, "6");
    setupScalesTab(tabs.tabScale7, scalesTab7, tabScale7, Settings::settingsScale7, "7");
    setupScalesTab(tabs.tabScale8, scalesTab8, tabScale8, Settings::settingsScale8, "8");
    setupScalesTab(tabs.tabScale9, scalesTab9, tabScale9, Settings::settingsScale9, "9");
    setupScalesTab(tabs.tabScale10, scalesTab10, tabScale10, Settings::settingsScale10, "10");

    // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
    // ESPUI.sliderContinuous = true;
    ESPUI.jsonUpdateDocumentSize = 3500; // 2000 is the default, and this value is not affected by the amount of widgets
    ESPUI.jsonInitialDocumentSize = 15000; // 8000 is the default, adjust when you have too many widgets or options
    ESPUI.begin("Smart Garden Config"); // , "username", "password")
    loop_espui();
}

void updateBatteryLevel() {
    readBatteryLevel(5);
    ESPUI.updateControlValue(status_akku, (String)getBatteryLevel() + "%");
}

void setupTabs() {
    tabs.tabNetwork = ESPUI.addControl( ControlType::Tab, "Network", "Network", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabs.tabBatteryLevel = ESPUI.addControl( ControlType::Tab, "Battery", "Battery", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabs.tabBME = ESPUI.addControl( ControlType::Tab, "Environment Sensor", "Environment Sensor", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabs.tabLux = ESPUI.addControl( ControlType::Tab, "Light Sensor", "Light Sensor", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabs.tabScale1 = ESPUI.addControl( ControlType::Tab, "Scale 1", "Scale 1", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale2 = ESPUI.addControl( ControlType::Tab, "Scale 2", "Scale 2", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale3 = ESPUI.addControl( ControlType::Tab, "Scale 3", "Scale 3", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale4 = ESPUI.addControl( ControlType::Tab, "Scale 4", "Scale 4", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale5 = ESPUI.addControl( ControlType::Tab, "Scale 5", "Scale 5", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale6 = ESPUI.addControl( ControlType::Tab, "Scale 6", "Scale 6", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale7 = ESPUI.addControl( ControlType::Tab, "Scale 7", "Scale 7", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale8 = ESPUI.addControl( ControlType::Tab, "Scale 8", "Scale 8", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale9 = ESPUI.addControl( ControlType::Tab, "Scale 9", "Scale 9", ControlColor::Alizarin,  Control::noParent, &switchTab);
    tabs.tabScale10 = ESPUI.addControl( ControlType::Tab, "Scale 10", "Scale 10", ControlColor::Alizarin,  Control::noParent, &switchTab);
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
    scalesTab.switchButton = ESPUI.addControl(ControlType::Switcher, "Use Scale", "0", ControlColor::Emerald, parentTab, &activateScale);
    tabScale.addElement<bool>(scalesTab.switchButton, &eepStructScale.ScaleInUse);
    scalesTab.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri", String("/home/smartGarden/scale") + scaleNumber, ControlColor::Emerald, parentTab, &updateValue);
    tabScale.addElement<String>(scalesTab.inputMqttUri, &eepStructScale.ScaleMqttUri);
    scalesTab.label = ESPUI.addControl(ControlType::Label, "current Value in kg", "", ControlColor::Emerald, parentTab);
    scalesTab.buttonTar = ESPUI.addControl(ControlType::Button, "Tar Scale", "Tar", ControlColor::Emerald, parentTab);
    scalesTab.buttonCalibrate = ESPUI.addControl(ControlType::Button, "Calibrate: Set current Value to 1kg", "Calibrate", ControlColor::Emerald, parentTab);
    scalesTab.labelTar = ESPUI.addControl(ControlType::Label, "Tar Value", "", ControlColor::Emerald, parentTab);
    tabScale.addElement<float>(scalesTab.labelTar, &eepStructScale.ScaleOffset);
    scalesTab.labelCalibrate = ESPUI.addControl(ControlType::Label, "Scaling Value", "", ControlColor::Emerald, parentTab);
    tabScale.addElement<float>(scalesTab.labelCalibrate, &eepStructScale.ScaleScale);
    scalesTab.buttonSave = ESPUI.addControl(ControlType::Button, "Save Scale Settings", "Save", ControlColor::Wetasphalt, parentTab, &saveTabCallback);
}

void loop_espui() {
    loadSettingsToUi();
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
    while (!ui_setup_finished) {
        dnsServer.processNextRequest();

        static long oldTime = 0;

        if (millis() - oldTime > 1000) {
            //ESPUI.updateControlValue(millisLabelId, String(millis()));
            //ESPUI.updateControlValue(status, String(batteryLevel));
            //ESPUI.updateControlValue(switchOne, testSwitchState ? "1" : "0");
            updateBatteryLevel();

            oldTime = millis();
        }
    }
    ESP.restart();
    delay(10000);
}

void setupBatteryTab(uint16_t parentTab) {
    batteryTab.batteryMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Battery Uri", "/home/balkon/batteryLevel/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBattery.addElement<String>(batteryTab.batteryMqttUri, &Settings::settingsBattery.MqttUri);
    batteryTab.batteryMqttUriLow = ESPUI.addControl(ControlType::Text, "MQTT Battery Uri Warning", "/home/balkon/batteryLow/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBattery.addElement<String>(batteryTab.batteryMqttUriLow, &Settings::settingsBattery.MqttUriLow);
}

void setupBMETab(uint16_t parentTab) {
    bmeTab.tempMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Temperature Uri", "/home/balkon/temp/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBme.addElement<String>(bmeTab.tempMqttUri, &Settings::settingsBme.MqttUriTemp);
    bmeTab.tempMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Humidity Uri", "/home/balkon/hum/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBme.addElement<String>(bmeTab.humMqttUri, &Settings::settingsBme.MqttUriHum);
    bmeTab.tempMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Pressure Uri", "/home/balkon/pres/", ControlColor::Peterriver, parentTab, &updateValue);
    tabBme.addElement<String>(bmeTab.presMqttUri, &Settings::settingsBme.MqttUriPres);
}

void setupLuxTab(uint16_t parentTab) {
    luxTab.luxMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Light Level Uri", "/home/balkon/lux/", ControlColor::Peterriver, parentTab, &updateValue);
    tabLux.addElement<String>(luxTab.luxMqttUri, &Settings::settingsLux.MqttUri);
}