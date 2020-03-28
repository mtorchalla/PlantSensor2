//
// Created by Moritz on 22.03.2020.
//

#ifndef PLANTSENSOR_SETTINGSMANAGER_H
#define PLANTSENSOR_SETTINGSMANAGER_H

#include <Arduino.h>
#include <Preferences.h>

template<typename T>
struct EepParameter {
    T value;
    const char *eepName;
};

typedef struct {
    EepParameter<float> ScaleOffset;
    EepParameter<float> ScaleScale;
    EepParameter<String> ScaleMqttUri;
    EepParameter<bool> ScaleInUse;
} EepStructScale;

typedef struct {
    EepParameter<String> WifiSSid;
    EepParameter<String> WifiPass;
    EepParameter<String> MqttServer;
    EepParameter<String> MqttUser;
    EepParameter<String> MqttPass;
    EepParameter<uint8_t> UpdateInterval;
} EepStructNetwork;

namespace Settings {

    extern EepStructNetwork settingsNetwork;
    extern EepStructScale settingsScale1;
    extern EepStructScale settingsScale2;
    extern EepStructScale settingsScale3;
    extern EepStructScale settingsScale4;
    extern EepStructScale settingsScale5;
    extern EepStructScale settingsScale6;
    extern EepStructScale settingsScale7;
    extern EepStructScale settingsScale8;
    extern EepStructScale settingsScale9;
    extern EepStructScale settingsScale10;

}

#define EEP_NETWORK_NAMESPACE "configNetwork"

class SettingsManager {
public:
    SettingsManager();

    void getConfigScalesAll();
    void getConfigScales(EepStructScale &scaleSettings);
    void getConfigNetwork(EepStructNetwork &networkSettings);
    void getConfigNetwork();
    void updateConfigScalesAll();
    void updateConfigScales(EepStructScale &eepStructScale);
    void updateConfigNetwork();
    void updateConfigNetwork(EepStructNetwork& networkSettings);
    boolean firstStartUp();
    boolean existScale(EepStructScale &eepStructScale);

private:
    Preferences preferences;
    void putString(EepParameter<String> &eepParameter);
    void putString(EepParameter<const char *> &eepParameter);
    void putUChar(EepParameter<uint8_t> &eepParameter);
    String getString(EepParameter<String> &eepParameter);
    String getString(const char* eepName);
    uint8_t getUChar(EepParameter<uint8_t> &eepParameter);
    uint8_t getUChar(const char *eepName);
};


#endif //PLANTSENSOR_SETTINGSMANAGER_H
