#include "SettingsManager.h"

//template<typename T, typename U>   //Typen ungleich, gibt 0 zurück
//struct is_same
//{
//    enum { value = 0 };
//};

EepStructNetwork Settings::settingsNetwork = {
        EepParameter<String>{"", "WifiSSid"},
        EepParameter<String>{"", "WifiPass"},
        EepParameter<String>{"", "MqttServer"},
        EepParameter<uint16_t>{1883, "MqttPort"},
        EepParameter<String>{"", "MqttUser"},
        EepParameter<String>{"", "MqttPass"},
        EepParameter<uint8_t>{15, "UpdateInterval"}
};

EepStructScale Settings::settingsScale1 = {EepParameter<float> {0.0, "ScaleOffset1"}, EepParameter<float> {0.0, "ScaleScale1"}, EepParameter<String> {"", "ScaleMqttUri1"}, EepParameter<bool> {false, "ScaleInUse1"}};
EepStructScale Settings::settingsScale2 = {EepParameter<float> {0.0, "ScaleOffset2"}, EepParameter<float> {0.0, "ScaleScale2"}, EepParameter<String> {"", "ScaleMqttUri2"}, EepParameter<bool> {false, "ScaleInUse2"}};
EepStructScale Settings::settingsScale3 = {EepParameter<float> {0.0, "ScaleOffset3"}, EepParameter<float> {0.0, "ScaleScale3"}, EepParameter<String> {"", "ScaleMqttUri3"}, EepParameter<bool> {false, "ScaleInUse3"}};
EepStructScale Settings::settingsScale4 = {EepParameter<float> {0.0, "ScaleOffset4"}, EepParameter<float> {0.0, "ScaleScale4"}, EepParameter<String> {"", "ScaleMqttUri4"}, EepParameter<bool> {false, "ScaleInUse4"}};
EepStructScale Settings::settingsScale5 = {EepParameter<float> {0.0, "ScaleOffset5"}, EepParameter<float> {0.0, "ScaleScale5"}, EepParameter<String> {"", "ScaleMqttUri5"}, EepParameter<bool> {false, "ScaleInUse5"}};
EepStructScale Settings::settingsScale6 = {EepParameter<float> {0.0, "ScaleOffset6"}, EepParameter<float> {0.0, "ScaleScale6"}, EepParameter<String> {"", "ScaleMqttUri6"}, EepParameter<bool> {false, "ScaleInUse6"}};
EepStructScale Settings::settingsScale7 = {EepParameter<float> {0.0, "ScaleOffset7"}, EepParameter<float> {0.0, "ScaleScale7"}, EepParameter<String> {"", "ScaleMqttUri7"}, EepParameter<bool> {false, "ScaleInUse7"}};
EepStructScale Settings::settingsScale8 = {EepParameter<float> {0.0, "ScaleOffset8"}, EepParameter<float> {0.0, "ScaleScale8"}, EepParameter<String> {"", "ScaleMqttUri8"}, EepParameter<bool> {false, "ScaleInUse8"}};
EepStructScale Settings::settingsScale9 = {EepParameter<float> {0.0, "ScaleOffset9"}, EepParameter<float> {0.0, "ScaleScale9"}, EepParameter<String> {"", "ScaleMqttUri9"}, EepParameter<bool> {false, "ScaleInUse9"}};
EepStructScale Settings::settingsScale10 = {EepParameter<float> {0.0, "ScaleOffset10"}, EepParameter<float> {0.0, "ScaleScale10"}, EepParameter<String> {"", "ScaleMqttUri10"}, EepParameter<bool> {false, "ScaleInUse10"}};


SettingsManager::SettingsManager() {
    preferences.begin("SettingsManager");
}

void SettingsManager::getConfigScalesAll() {
    this->getConfigScales(Settings::settingsScale1);
    this->getConfigScales(Settings::settingsScale2);
    this->getConfigScales(Settings::settingsScale3);
    this->getConfigScales(Settings::settingsScale4);
    this->getConfigScales(Settings::settingsScale5);
    this->getConfigScales(Settings::settingsScale6);
    this->getConfigScales(Settings::settingsScale7);
    this->getConfigScales(Settings::settingsScale8);
    this->getConfigScales(Settings::settingsScale9);
    this->getConfigScales(Settings::settingsScale10);
}

void SettingsManager::getConfigScales(EepStructScale &scaleSettings) {
    if (!this->firstStartUp()) {
        scaleSettings.ScaleOffset.value = this->preferences.getFloat(scaleSettings.ScaleOffset.eepName, 0);
        scaleSettings.ScaleScale.value = this->preferences.getFloat(scaleSettings.ScaleScale.eepName, 0);
        scaleSettings.ScaleMqttUri.value = this->getString(scaleSettings.ScaleMqttUri);
        scaleSettings.ScaleInUse.value = this->preferences.getBool(scaleSettings.ScaleInUse.eepName, 0);
    }
}

void SettingsManager::getConfigNetwork() {
    this->getConfigNetwork(Settings::settingsNetwork);
}

void SettingsManager::getConfigNetwork(EepStructNetwork &networkSettings) {
    if (!this->firstStartUp()) {
        networkSettings.WifiSSid.value = this->getString(networkSettings.WifiSSid.eepName);
        networkSettings.WifiPass.value = this->getString(networkSettings.WifiPass.eepName);
        networkSettings.MqttServer.value = this->getString(networkSettings.MqttServer.eepName);
        networkSettings.MqttPort.value = this->preferences.getShort(networkSettings.MqttServer.eepName);
        networkSettings.MqttUser.value = this->getString(networkSettings.MqttUser.eepName);
        networkSettings.MqttPass.value = this->getString(networkSettings.MqttPass.eepName);
        networkSettings.UpdateInterval.value = this->getUChar(networkSettings.UpdateInterval.eepName);
    }
//    if (dataLen == sizeof(EepStructNetwork)) { // simple check that data fits
//        return nullptr; // data for scaleNr is not set, first startup!
//    }
//    auto* networkSettings = (EepStructNetwork*) buffer; // cast the bytes into a struct ptr
}

void SettingsManager::updateConfigScalesAll() {
    this->updateConfigScales(Settings::settingsScale1);
    this->updateConfigScales(Settings::settingsScale2);
    this->updateConfigScales(Settings::settingsScale3);
    this->updateConfigScales(Settings::settingsScale4);
    this->updateConfigScales(Settings::settingsScale5);
    this->updateConfigScales(Settings::settingsScale6);
    this->updateConfigScales(Settings::settingsScale7);
    this->updateConfigScales(Settings::settingsScale8);
    this->updateConfigScales(Settings::settingsScale9);
    this->updateConfigScales(Settings::settingsScale10);
}

void SettingsManager::updateConfigScales(EepStructScale &eepStructScale) {
    this->putString(eepStructScale.ScaleMqttUri);
    this->preferences.putFloat(eepStructScale.ScaleScale.eepName, eepStructScale.ScaleScale.value);
    this->preferences.putFloat(eepStructScale.ScaleOffset.eepName, eepStructScale.ScaleOffset.value);
    this->preferences.putBool(eepStructScale.ScaleInUse.eepName, eepStructScale.ScaleInUse.value);
}

void SettingsManager::updateConfigNetwork() {
    this->updateConfigNetwork(Settings::settingsNetwork);
}

void SettingsManager::updateConfigNetwork(EepStructNetwork &networkSettings) {
    this->putString(networkSettings.WifiSSid);
    this->putString(networkSettings.WifiPass);
    this->putString(networkSettings.MqttServer);
    this->preferences.putShort(networkSettings.MqttPort.eepName, networkSettings.MqttPort.value);
    this->putString(networkSettings.MqttUser);
    this->putString(networkSettings.MqttPass);
    this->putUChar(networkSettings.UpdateInterval);
}


void SettingsManager::putString(EepParameter<String> &eepParameter) {
    this->preferences.putString(eepParameter.eepName, eepParameter.value);
}

void SettingsManager::putString(EepParameter<const char *> &eepParameter) {
    this->preferences.putString(eepParameter.eepName, eepParameter.value);
}

void SettingsManager::putUChar(EepParameter<uint8_t> &eepParameter) {
    this->preferences.putUChar(eepParameter.eepName, eepParameter.value);
}

String SettingsManager::getString(EepParameter<String> &eepParameter) {
    return this->preferences.getString(eepParameter.eepName, "");
}

String SettingsManager::getString(const char *eepName) {
    return this->preferences.getString(eepName, "");
}

uint8_t SettingsManager::getUChar(EepParameter<uint8_t> &eepParameter) {
    return this->preferences.getUChar(eepParameter.eepName, 0);
}

uint8_t SettingsManager::getUChar(const char *eepName) {
    return this->preferences.getUChar(eepName, 0);
}

boolean SettingsManager::firstStartUp() {
    return this->getString("WifiSSid").isEmpty();
}

boolean SettingsManager::existScale(EepStructScale &eepStructScale) {
    return this->preferences.getFloat(eepStructScale.ScaleInUse.eepName, 0);
}

//template<typename T>
//void SettingsManager::saveValue(EepParameter<T> &eepParameter) {
//    if (is_same<uint8_t, decltype(eepParameter.value)>::value) {
//        this->preferences.putUChar(eepParameter.eepName, eepParameter.value);
//    }
//    if (is_same<float, decltype(eepParameter.value)>::value) {
//        this->preferences.putFloat(eepParameter.eepName, eepParameter.value);
//    }
//    if (is_same<String, decltype(eepParameter.value)>::value) {
//        this->preferences.putString(eepParameter.eepName, eepParameter.value);
//    }
//    if (is_same<const char*, decltype(eepParameter.value)>::value) {
//        this->preferences.putString(eepParameter.eepName, eepParameter.value);
//    }
//}

template<> bool SettingsManager::loadValue(EepParameter<bool> &eepParameter) {
    log_i("SettingsManager: Loading Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    return this->preferences.getBool(eepParameter.eepName, 0);
}
template<> uint8_t SettingsManager::loadValue(EepParameter<uint8_t > &eepParameter) {
    log_i("SettingsManager: Loading Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    return this->preferences.getUChar(eepParameter.eepName, 0);
}
template<> uint16_t SettingsManager::loadValue(EepParameter<uint16_t > &eepParameter) {
    log_i("SettingsManager: Loading Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    return this->preferences.getShort(eepParameter.eepName, 0);
}
template<> float SettingsManager::loadValue(EepParameter<float > &eepParameter) {
    log_i("SettingsManager: Loading Parameter: %s; Value: %f", eepParameter.eepName, eepParameter.value);
    return this->preferences.getFloat(eepParameter.eepName, 0);
}
template<> String SettingsManager::loadValue(EepParameter<String> &eepParameter) {
    log_i("SettingsManager: Loading Parameter: %s; Value: %s", eepParameter.eepName, eepParameter.value);
    return this->preferences.getString(eepParameter.eepName, "");
}

template<> void SettingsManager::saveValue(EepParameter<bool> &eepParameter) {
    log_i("SettingsManager: Saving Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    this->preferences.putBool(eepParameter.eepName, eepParameter.value);
}
template<> void SettingsManager::saveValue(EepParameter<uint8_t> &eepParameter) {
    log_i("SettingsManager: Saving Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    this->preferences.putUChar(eepParameter.eepName, eepParameter.value);
}
template<> void SettingsManager::saveValue(EepParameter<uint16_t> &eepParameter) {
    log_i("SettingsManager: Saving Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    this->preferences.putShort(eepParameter.eepName, eepParameter.value);
}
template<> void SettingsManager::saveValue(EepParameter<float> &eepParameter) {
    log_i("SettingsManager: Saving Parameter: %s; Value: %f", eepParameter.eepName, eepParameter.value);
    this->preferences.putFloat(eepParameter.eepName, eepParameter.value);
}
template<> void SettingsManager::saveValue(EepParameter<String> &eepParameter) {
    log_i("SettingsManager: Saving Parameter: %s; Value: %s", eepParameter.eepName, eepParameter.value);
    this->preferences.putString(eepParameter.eepName, eepParameter.value);
}


//template<typename T>
//T SettingsManager::loadValue(EepParameter<T> &eepParameter) {
//    if (is_same<uint8_t, decltype(eepParameter.value)>::value) {
//        return (T)this->preferences.getUChar(eepParameter.eepName, 0);
//    }
//    if (is_same<float, decltype(eepParameter.value)>::value) {
//        return (T)this->preferences.getFloat(eepParameter.eepName, 0);
//    }
//    if (is_same<String, decltype(eepParameter.value)>::value) {
//        return static_cast<T>(this->preferences.getString(eepParameter.eepName, ""));
//    }
//    if (is_same<const char*, decltype(eepParameter.value)>::value) {
//        return (T)this->preferences.getString(eepParameter.eepName, "");
//    }
//}


