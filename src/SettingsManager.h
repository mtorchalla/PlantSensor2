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
    EepParameter<uint16_t> MqttPort;
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
    template<typename T>
    void saveValue(EepParameter<T> &eepParameter);
    template<typename T>
    T loadValue(EepParameter<T> &eepParameter);

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

template<> uint8_t SettingsManager::loadValue(EepParameter<uint8_t > &eepParameter);
template<> uint16_t SettingsManager::loadValue(EepParameter<uint16_t > &eepParameter);
template<> float SettingsManager::loadValue(EepParameter<float > &eepParameter);
template<> String SettingsManager::loadValue(EepParameter<String> &eepParameter);

template<> void SettingsManager::saveValue(EepParameter<uint8_t > &eepParameter);
template<> void SettingsManager::saveValue(EepParameter<uint16_t > &eepParameter);
template<> void SettingsManager::saveValue(EepParameter<float > &eepParameter);
template<> void SettingsManager::saveValue(EepParameter<String> &eepParameter);

#endif //PLANTSENSOR_SETTINGSMANAGER_H
