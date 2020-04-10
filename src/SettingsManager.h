#ifndef PLANTSENSOR_SETTINGSMANAGER_H
#define PLANTSENSOR_SETTINGSMANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include "ConfigScale.h"
#include "EepParameterTypes.h"


typedef struct {
    EepParameter<String> WifiSSid;
    EepParameter<String> WifiPass;
    EepParameter<String> MqttServer;
    EepParameter<uint16_t> MqttPort;
    EepParameter<String> MqttUser;
    EepParameter<String> MqttPass;
    EepParameter<String> MqttName;
    EepParameter<uint8_t> UpdateInterval;
} EepStructNetwork;

typedef struct {
    EepParameter<String> MqttUri;
    EepParameter<String> MqttUriLow;
} EepStructBattery;

typedef struct {
    EepParameter<String> MqttUriTemp;
    EepParameter<String> MqttUriHum;
    EepParameter<String> MqttUriPres;
} EepStructBme;

typedef struct {
    EepParameter<String> MqttUri;
} EepStructLux;

typedef struct {
    EepParameter<bool> useValve1;
    EepParameter<bool> useValve2;
    EepParameter<bool> useValve3;
    EepParameter<bool> useValve4;
    EepParameter<bool> useValve5;
    EepParameter<bool> useValve6;
    EepParameter<bool> useValve7;
    EepParameter<bool> useValve8;
    EepParameter<bool> useValve9;
    EepParameter<bool> useValve10;
    EepParameter<float> voltageValve1;
    EepParameter<float> voltageValve2;
    EepParameter<float> voltageValve3;
    EepParameter<float> voltageValve4;
    EepParameter<float> voltageValve5;
    EepParameter<float> voltageValve6;
    EepParameter<float> voltageValve7;
    EepParameter<float> voltageValve8;
    EepParameter<float> voltageValve9;
    EepParameter<float> voltageValve10;
    EepParameter<uint8_t> selectScale1;
    EepParameter<uint8_t> selectScale2;
    EepParameter<uint8_t> selectScale3;
    EepParameter<uint8_t> selectScale4;
    EepParameter<uint8_t> selectScale5;
    EepParameter<uint8_t> selectScale6;
    EepParameter<uint8_t> selectScale7;
    EepParameter<uint8_t> selectScale8;
    EepParameter<uint8_t> selectScale9;
    EepParameter<uint8_t> selectScale10;
    EepParameter<String> mqttUriValve1;
    EepParameter<String> mqttUriValve2;
    EepParameter<String> mqttUriValve3;
    EepParameter<String> mqttUriValve4;
    EepParameter<String> mqttUriValve5;
    EepParameter<String> mqttUriValve6;
    EepParameter<String> mqttUriValve7;
    EepParameter<String> mqttUriValve8;
    EepParameter<String> mqttUriValve9;
    EepParameter<String> mqttUriValve10;
    EepParameter<bool> *useValveList[NR_OF_SCALES];
    EepParameter<float> *voltageValveList[NR_OF_SCALES];
    EepParameter<uint8_t> *selectScaleList[NR_OF_SCALES];
    EepParameter<String> *mqttUriList[NR_OF_SCALES];
} EepStructValves;

namespace Settings {

    extern EepStructNetwork settingsNetwork;
    extern EepStructBattery settingsBattery;
    extern EepStructLux settingsLux;
    extern EepStructBme settingsBme;
    extern EepStructValves settingsValves;
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
    void getConfigBattery();
    void getConfigLux();
    void getConfigBme();
    void getConfigValves();
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
