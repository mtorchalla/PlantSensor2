#include <Arduino.h>
#include <DNSServer.h>
#include <ESPUI.h>
#include <WiFi.h>
#include <nvs_flash.h>
#include "WebUi.h"
#include "PlantSensor.h"
#include "ConfigAkku.h"
#include "SettingsManager.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
const char *ESPUI_ssid = "ESPUI";
const char *ESPUI_password = "espui";
const char *ESPUI_hostname = "espui";


uint16_t activeTab = 0;
uint16_t status_akku = 0;
uint16_t button_save = 0;
uint16_t status_log = 0;
SettingsManager settingsManager;

struct tabsStruct {
    uint16_t tabNetwork;
    uint16_t tabScales;
} tabs;

struct networkTabStruct {
    uint16_t textSsid;
    uint16_t textPass;
    uint16_t textMqttServer;
    uint16_t textMqttUser;
    uint16_t textMqttPass;
    uint16_t sliderUpdate;
    uint16_t buttonSave;
} networkTab;

struct scalesTabStruct {
    scaleSettings scale1;
    scaleSettings scale2;
    scaleSettings scale3;
    scaleSettings scale4;
    scaleSettings scale5;
    scaleSettings scale6;
    scaleSettings scale7;
    scaleSettings scale8;
    scaleSettings scale9;
    scaleSettings scale10;
    uint16_t buttonSave;
} scalesTab;

uint16_t button1;
uint16_t millisLabelId;
uint16_t switchOne;


void saveAndQuit(Control *sender, int type) {
    if (ESPUI.getControl(networkTab.textSsid)->value.isEmpty()) {
        ESPUI.getControl(status_log)->value = "Enter at least a Wifi-SSID!";
        ESPUI.updateControl(status_log);
        return;
    }
    ESPUI.getControl(status_log)->value = "Saving values and rebooting...";
    ESPUI.updateControl(status_log);
}

void loadSettingsToUi() {
    Serial.println("Getting Network Settings");
    settingsManager.getConfigNetwork();
    Serial.println(Settings::settingsNetwork.WifiSSid.value);
    Serial.println(settingsManager.firstStartUp());
    if (!settingsManager.firstStartUp()) {
        loadNetworkSettings();
        loadScaleSettingsAll();
    }
}

void loadScaleSettingsAll() {
    settingsManager.getConfigScalesAll();
    loadScaleSettings(scalesTab.scale1, Settings::settingsScale1);
    loadScaleSettings(scalesTab.scale2, Settings::settingsScale2);
    loadScaleSettings(scalesTab.scale3, Settings::settingsScale3);
    loadScaleSettings(scalesTab.scale4, Settings::settingsScale4);
    loadScaleSettings(scalesTab.scale5, Settings::settingsScale5);
    loadScaleSettings(scalesTab.scale6, Settings::settingsScale6);
    loadScaleSettings(scalesTab.scale7, Settings::settingsScale7);
    loadScaleSettings(scalesTab.scale8, Settings::settingsScale8);
    loadScaleSettings(scalesTab.scale9, Settings::settingsScale9);
    loadScaleSettings(scalesTab.scale10, Settings::settingsScale10);
}

void loadScaleSettings(scaleSettings &scaleUiStruct, EepStructScale &eepStructScale) {
    ESPUI.getControl(scaleUiStruct.switchScale)->value = (String)eepStructScale.ScaleInUse.value;
    scaleUiStruct.calculatedScale = eepStructScale.ScaleScale.value;
    scaleUiStruct.calculatedOffset = eepStructScale.ScaleOffset.value;
    if (!(String)eepStructScale.ScaleMqttUri.value.isEmpty()) {
        ESPUI.getControl(scaleUiStruct.inputMqttUri)->value = (String)eepStructScale.ScaleMqttUri.value;
    }

    ESPUI.updateControl(scaleUiStruct.switchScale);
    ESPUI.updateControl(scaleUiStruct.inputMqttUri);
}

void saveTabScales(Control *sender, int type) {
    saveScaleToSettings(scalesTab.scale1, Settings::settingsScale1);
    saveScaleToSettings(scalesTab.scale2, Settings::settingsScale2);
    saveScaleToSettings(scalesTab.scale3, Settings::settingsScale3);
    saveScaleToSettings(scalesTab.scale4, Settings::settingsScale4);
    saveScaleToSettings(scalesTab.scale5, Settings::settingsScale5);
    saveScaleToSettings(scalesTab.scale6, Settings::settingsScale6);
    saveScaleToSettings(scalesTab.scale7, Settings::settingsScale7);
    saveScaleToSettings(scalesTab.scale8, Settings::settingsScale8);
    saveScaleToSettings(scalesTab.scale9, Settings::settingsScale9);
    saveScaleToSettings(scalesTab.scale10, Settings::settingsScale10);
    settingsManager.updateConfigScalesAll();
}

void saveTabNetwork(Control *sender, int type) {
    Settings::settingsNetwork.WifiSSid.value = ESPUI.getControl(networkTab.textSsid)->value;
    Settings::settingsNetwork.WifiPass.value = ESPUI.getControl(networkTab.textPass)->value;
    Settings::settingsNetwork.MqttServer.value = ESPUI.getControl(networkTab.textMqttServer)->value;
    Settings::settingsNetwork.MqttUser.value = ESPUI.getControl(networkTab.textMqttUser)->value;
    Settings::settingsNetwork.MqttPass.value = ESPUI.getControl(networkTab.textMqttPass)->value;
    settingsManager.updateConfigNetwork();
}


void loadNetworkSettings() {
    settingsManager.getConfigNetwork();
    if (!settingsManager.firstStartUp()) {
        ESPUI.getControl(networkTab.textSsid)->value = Settings::settingsNetwork.WifiSSid.value;
        ESPUI.getControl(networkTab.textPass)->value = Settings::settingsNetwork.WifiPass.value;
        ESPUI.getControl(networkTab.textMqttServer)->value = Settings::settingsNetwork.MqttServer.value;
        ESPUI.getControl(networkTab.textMqttUser)->value = Settings::settingsNetwork.MqttUser.value;
        ESPUI.getControl(networkTab.textMqttPass)->value = Settings::settingsNetwork.MqttPass.value;
        ESPUI.getControl(networkTab.sliderUpdate)->value = (String)Settings::settingsNetwork.UpdateInterval.value;

        ESPUI.updateControl(networkTab.textSsid);
        ESPUI.updateControl(networkTab.textPass);
        ESPUI.updateControl(networkTab.textMqttServer);
        ESPUI.updateControl(networkTab.textMqttUser);
        ESPUI.updateControl(networkTab.sliderUpdate);

    }
}

void saveScaleToSettings(scaleSettings &scaleUiStruct, EepStructScale &eepStructScale) {
    eepStructScale.ScaleInUse.value = (boolean)ESPUI.getControl(scaleUiStruct.switchScale)->value;
    eepStructScale.ScaleMqttUri.value = ESPUI.getControl(scaleUiStruct.inputMqttUri)->value;
    eepStructScale.ScaleScale.value = scaleUiStruct.calculatedScale;
    eepStructScale.ScaleOffset.value = scaleUiStruct.calculatedOffset;
}

void textCallMy(Control *sender, int type) {
    Serial.print("Text: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void switchTab(Control* sender, int type) {
    activeTab = sender->id;
}

void activateScale(Control* sender, int type) {
    if (sender->id == scalesTab.scale1.switchScale) switchScaleElements(scalesTab.scale1, sender->value);
    if (sender->id == scalesTab.scale2.switchScale) switchScaleElements(scalesTab.scale2, sender->value);
    if (sender->id == scalesTab.scale3.switchScale) switchScaleElements(scalesTab.scale3, sender->value);
    if (sender->id == scalesTab.scale4.switchScale) switchScaleElements(scalesTab.scale4, sender->value);
    if (sender->id == scalesTab.scale5.switchScale) switchScaleElements(scalesTab.scale5, sender->value);
    if (sender->id == scalesTab.scale6.switchScale) switchScaleElements(scalesTab.scale6, sender->value);
    if (sender->id == scalesTab.scale7.switchScale) switchScaleElements(scalesTab.scale7, sender->value);
    if (sender->id == scalesTab.scale8.switchScale) switchScaleElements(scalesTab.scale8, sender->value);
    if (sender->id == scalesTab.scale9.switchScale) switchScaleElements(scalesTab.scale9, sender->value);
    if (sender->id == scalesTab.scale10.switchScale) switchScaleElements(scalesTab.scale10, sender->value);
}

void switchScaleElements(scaleSettings &scaleStruct, String value) {
    if (value == "1") {
        ESPUI.getControl(scaleStruct.labelScale)->color = ControlColor::Emerald;
        ESPUI.getControl(scaleStruct.labelScale)->value = "Getting Scale Value...";
        ESPUI.getControl(scaleStruct.buttonTarScale)->color = ControlColor::Emerald;
        ESPUI.getControl(scaleStruct.buttonCalibrateScale)->color = ControlColor::Emerald;
        ESPUI.getControl(scaleStruct.inputMqttUri)->color = ControlColor::Emerald;
    }
    else {
        ESPUI.getControl(scaleStruct.labelScale)->color = ControlColor::Dark;
        ESPUI.getControl(scaleStruct.labelScale)->value = "Not used";
        ESPUI.getControl(scaleStruct.buttonTarScale)->color = ControlColor::Dark;
        ESPUI.getControl(scaleStruct.buttonCalibrateScale)->color = ControlColor::Dark;
        ESPUI.getControl(scaleStruct.inputMqttUri)->color = ControlColor::Dark;
    }
    ESPUI.updateControl(scaleStruct.labelScale);
    ESPUI.updateControl(scaleStruct.buttonTarScale);
    ESPUI.updateControl(scaleStruct.buttonCalibrateScale);
    ESPUI.updateControl(scaleStruct.inputMqttUri);
}

void sliderMy(Control *sender, int type) {
    Serial.print("Slider: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void buttonCallbackMy(Control *sender, int type) {
    switch (type) {
        case B_DOWN:
            Serial.println("Button DOWN");
            break;

        case B_UP:
            Serial.println("Button UP");
            break;
    }
}

void buttonExampleMy(Control *sender, int type) {
    switch (type) {
        case B_DOWN:
            Serial.println("Status: Start");
//            ESPUI.updateControlValue(status_akku, "Start");

            ESPUI.getControl(button1)->color = ControlColor::Carrot;
            ESPUI.updateControl(button1);
            break;

        case B_UP:
            Serial.println("Status: Stop");
//            ESPUI.updateControlValue(status, "Stop");

            ESPUI.getControl(button1)->color = ControlColor::Peterriver;
            ESPUI.updateControl(button1);
            break;
    }
}

void switchExample(Control *sender, int value) {
    switch (value) {
        case S_ACTIVE:
            Serial.print("Active:");
            break;

        case S_INACTIVE:
            Serial.print("Inactive");
            break;
    }

    Serial.print(" ");
    Serial.println(sender->id);
}

void selectExample(Control *sender, int value) {
    Serial.print("Select: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void setup_espui(SettingsManager& newSettingsManager) {
    settingsManager = newSettingsManager;
    Serial.println(2);
    Serial.println(settingsManager.firstStartUp());
    ESPUI.setVerbosity(ESPUI_Verbosity::VerboseJSON);


    WiFi.setHostname(ESPUI_hostname);

    // try to connect to existing network
    WiFi.begin(ESPUI_ssid, ESPUI_password);
//    Serial.print("\n\nTry to connect to existing network");

    uint8_t timeout = 10;
    // create hotspot
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("\n\nCreating hotspot");

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
        WiFi.softAP(ESPUI_ssid);

        timeout = 5;

        do {
            delay(500);
            Serial.print(".");
            timeout--;
        } while (timeout);
    }

    dnsServer.start(DNS_PORT, "*", apIP);

    Serial.println("\n\nWiFi parameters:");
    Serial.print("Mode: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

    status_akku = ESPUI.addControl(ControlType::Label, "Battery Level:", cakku, ControlColor::Sunflower);
    button_save = ESPUI.addControl(ControlType::Button, "Save and Reboot", "Save", ControlColor::Emerald, Control::noParent, &saveAndQuit);
    status_log = ESPUI.addControl(ControlType::Label, "Status Message", "Please enter your data", ControlColor::Sunflower);
//    update_akku();


    setupTabs();
    setupNetworkTab(tabs.tabNetwork);
    setupScalesTab(tabs.tabScales);
//    uint16_t select1 = ESPUI.addControl(ControlType::Select, "Select:", "", ControlColor::Alizarin,
//                                        tabNetwork, &selectExample);
//    ESPUI.addControl(ControlType::Option, "Option1", "Opt1", ControlColor::Alizarin, select1);
//    ESPUI.addControl(ControlType::Option, "Option2", "Opt2", ControlColor::Alizarin, select1);
//    ESPUI.addControl(ControlType::Option, "Option3", "Opt3", ControlColor::Alizarin, select1);
//
//    ESPUI.addControl(ControlType::Text, "Text Test:", "a Text Field", ControlColor::Alizarin, tabScales,
//                     &textCallMy);
//
//    millisLabelId = ESPUI.addControl(ControlType::Label, "Millis:", "0", ControlColor::Emerald, tabNetwork);
//    button1 = ESPUI.addControl(ControlType::Button, "Push Button", "Press", ControlColor::Peterriver,
//                               tabScales, &buttonCallbackMy);
//    ESPUI.addControl(ControlType::Button, "Other Button", "Press", ControlColor::Wetasphalt, tabNetwork,
//                     &buttonCallbackMy);
//    switchOne = ESPUI.addControl(ControlType::Switcher, "Switch one", "", ControlColor::Alizarin, tabScales,
//                                 &switchExample);
//    ESPUI.addControl(ControlType::Slider, "Slider one", "30", ControlColor::Alizarin, tabNetwork, &sliderMy);
//    ESPUI.addControl(ControlType::Slider, "Slider two", "100", ControlColor::Alizarin, tabScales, &sliderMy);
//    ESPUI.addControl(ControlType::Number, "Number:", "50", ControlColor::Alizarin, tabScales, &numberCallMy);

    /*
     * .begin loads and serves all files from PROGMEM directly.
     * If you want to serve the files from SPIFFS use ESPUI.beginSPIFFS
     * (.prepareFileSystem has to be run in an empty sketch before)
     */

    // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
    // ESPUI.sliderContinuous = true;

    /*
     * Optionally you can use HTTP BasicAuth. Keep in mind that this is NOT a
     * SECURE way of limiting access.
     * Anyone who is able to sniff traffic will be able to intercept your password
     * since it is transmitted in cleartext. Just add a string as username and
     * password, for example begin("ESPUI Control", "username", "password")
     */

    ESPUI.begin("Smart Garden Config");
    loop_espui();
}

void update_akku() {
//    readAkku(5); // TODO
    ESPUI.updateControlValue(status_akku, cakku);
}

void setupTabs() {
    tabs.tabNetwork = ESPUI.addControl( ControlType::Tab, "Network", "Network", ControlColor::Alizarin, Control::noParent, &switchTab);
    tabs.tabScales = ESPUI.addControl( ControlType::Tab, "Scales", "Scales", ControlColor::Alizarin,  Control::noParent, &switchTab);
}

void setupNetworkTab(uint16_t parentTab) {
    networkTab.textSsid = ESPUI.addControl(ControlType::Text, "Network SSID", "",
            ControlColor::Peterriver, parentTab, &textCallMy);
    networkTab.textPass = ESPUI.addControl(ControlType::Text, "Network Password", "",
            ControlColor::Peterriver, parentTab, &textCallMy);
    networkTab.textMqttServer = ESPUI.addControl(ControlType::Text, "MQTT Server Address", "",
                                           ControlColor::Peterriver, parentTab, &textCallMy);
    networkTab.textMqttUser = ESPUI.addControl(ControlType::Text, "MQTT User Name", "",
                                           ControlColor::Peterriver, parentTab, &textCallMy);
    networkTab.textMqttPass = ESPUI.addControl(ControlType::Text, "MQTT Password", "",
                                           ControlColor::Peterriver, parentTab, &textCallMy);
    networkTab.sliderUpdate = ESPUI.addControl(ControlType::Slider, "Update Interval in Minutes (Smaller Values = more Power Consumption)", "10", ControlColor::Peterriver, parentTab, &sliderMy);
    networkTab.buttonSave = ESPUI.addControl(ControlType::Button, "Save", "Save",
            ControlColor::Wetasphalt, parentTab, &saveTabNetwork);
}

void setupScalesTab(uint16_t parentTab) {
    scalesTab.scale1.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 1", "1", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale1.labelScale = ESPUI.addControl(ControlType::Label, "Scale 1 Value", "Scale 1 Value", ControlColor::Emerald, parentTab);
    scalesTab.scale1.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 1", "Tar Scale 1", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale1.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 1 Value to 1kg", "Calibrate1", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale1.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 1", "/home/smartGarden/scale1", ControlColor::Emerald, parentTab);
    ESPUI.addControl(ControlType::Step, "", "");

    scalesTab.scale2.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 2", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale2.labelScale = ESPUI.addControl(ControlType::Label, "Scale 2 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale2.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 2", "Tar Scale 2", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale2.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 2 Value to 1kg", "Calibrate2", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale2.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 2", "/home/smartGarden/scale2", ControlColor::Emerald, parentTab);

    scalesTab.scale3.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 3", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale3.labelScale = ESPUI.addControl(ControlType::Label, "Scale 3 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale3.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 3", "Tar Scale 3", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale3.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 3 Value to 1kg", "Calibrate3", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale3.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 3", "/home/smartGarden/scale3", ControlColor::Emerald, parentTab);

    scalesTab.scale4.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 4", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale4.labelScale = ESPUI.addControl(ControlType::Label, "Scale 4 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale4.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 4", "Tar Scale 4", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale4.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 4 Value to 1kg", "Calibrate4", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale4.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 4", "/home/smartGarden/scale4", ControlColor::Emerald, parentTab);

    scalesTab.scale5.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 5", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale5.labelScale = ESPUI.addControl(ControlType::Label, "Scale 5 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale5.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 5", "Tar Scale 5", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale5.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 5 Value to 1kg", "Calibrate5", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale5.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 5", "/home/smartGarden/scale5", ControlColor::Emerald, parentTab);

    scalesTab.scale6.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 6", "6", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale6.labelScale = ESPUI.addControl(ControlType::Label, "Scale 6 Value", "Scale Value", ControlColor::Emerald, parentTab);
    scalesTab.scale6.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 6", "Tar Scale 6", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale6.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 6 Value to 1kg", "Calibrate6", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale6.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 6", "/home/smartGarden/scale6", ControlColor::Emerald, parentTab);

    scalesTab.scale7.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 7", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale7.labelScale = ESPUI.addControl(ControlType::Label, "Scale 7 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale7.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 7", "Tar Scale 7", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale7.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 7 Value to 1kg", "Calibrate7", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale7.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 7", "/home/smartGarden/scale7", ControlColor::Emerald, parentTab);

    scalesTab.scale8.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 8", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale8.labelScale = ESPUI.addControl(ControlType::Label, "Scale 8 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale8.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 8", "Tar Scale 8", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale8.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 8 Value to 1kg", "Calibrate8", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale8.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 8", "/home/smartGarden/scale8", ControlColor::Emerald, parentTab);

    scalesTab.scale9.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 9", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale9.labelScale = ESPUI.addControl(ControlType::Label, "Scale 9 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale9.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 9", "Tar Scale 9", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale9.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 9 Value to 1kg", "Calibrate9", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale9.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 9", "/home/smartGarden/scale9", ControlColor::Emerald, parentTab);

    scalesTab.scale10.switchScale = ESPUI.addControl(ControlType::Switcher, "Use Scale 10", "", ControlColor::Emerald, parentTab, &activateScale);
    scalesTab.scale10.labelScale = ESPUI.addControl(ControlType::Label, "Scale 10 Value", "", ControlColor::Emerald, parentTab);
    scalesTab.scale10.buttonTarScale = ESPUI.addControl(ControlType::Button, "Tar Scale 10", "Tar Scale 10", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale10.buttonCalibrateScale = ESPUI.addControl(ControlType::Button, "Calibrate current Scale 10 Value to 1kg", "Calibrate10", ControlColor::Emerald, parentTab, &buttonCallbackMy);
    scalesTab.scale10.inputMqttUri = ESPUI.addControl(ControlType::Text, "MQTT Uri Scale 10", "/home/smartGarden/scale10", ControlColor::Emerald, parentTab);

    scalesTab.buttonSave = ESPUI.addControl(ControlType::Button, "Save Scale Settings", "SaveScales", ControlColor::Wetasphalt, parentTab, &saveTabScales);
}


void loop_espui() {
    loadSettingsToUi();
    while (1) {
        dnsServer.processNextRequest();

        static long oldTime = 0;

        if (millis() - oldTime > 1000) {
            //ESPUI.updateControlValue(millisLabelId, String(millis()));
            //ESPUI.updateControlValue(status, String(akku));
            //ESPUI.updateControlValue(switchOne, testSwitchState ? "1" : "0");
            update_akku();

            oldTime = millis();
        }
    }
}