#include "SettingsManager.h"

//template<typename T, typename U>   //Typen ungleich, gibt 0 zurück
//struct is_same
//{
//    enum { value = 0 };
//};

RTC_DATA_ATTR EepStructNetwork Settings::settingsNetwork = {
        EepParameter<String>{"", "WifiSSid"},
        EepParameter<String>{"", "WifiPass"},
        EepParameter<String>{"", "MqttServer"},
        EepParameter<uint16_t>{1883, "MqttPort"},
        EepParameter<String>{"", "MqttUser"},
        EepParameter<String>{"", "MqttPass"},
        EepParameter<String>{"", "MqttName"},
        EepParameter<uint8_t>{15, "UpdateInterval"}
};

RTC_DATA_ATTR EepStructBattery Settings::settingsBattery = { EepParameter<String> { "", "MqttUriBat" }, EepParameter<String> { "", "MqttUriBatLow" } };
RTC_DATA_ATTR EepStructBme Settings::settingsBme = { EepParameter<String> { "", "MqttUriTemp" }, EepParameter<String> { "", "MqttUriHum" }, EepParameter<String> { "", "MqttUriPres" } };
RTC_DATA_ATTR EepStructLux Settings::settingsLux = { EepParameter<String> { "", "MqttUriLux" } };
RTC_DATA_ATTR EepStructValves Settings::settingsValves = {
        EepParameter<bool> {false, "useValve1"},
        EepParameter<bool> {false, "useValve2"},
        EepParameter<bool> {false, "useValve3"},
        EepParameter<bool> {false, "useValve4"},
        EepParameter<bool> {false, "useValve5"},
        EepParameter<bool> {false, "useValve6"},
        EepParameter<bool> {false, "useValve7"},
        EepParameter<bool> {false, "useValve8"},
        EepParameter<bool> {false, "useValve9"},
        EepParameter<bool> {false, "useValve10"},
        EepParameter<float> {5.0, "voltageValve1"},
        EepParameter<float> {5.0, "voltageValve2"},
        EepParameter<float> {5.0, "voltageValve3"},
        EepParameter<float> {5.0, "voltageValve4"},
        EepParameter<float> {5.0, "voltageValve5"},
        EepParameter<float> {5.0, "voltageValve6"},
        EepParameter<float> {5.0, "voltageValve7"},
        EepParameter<float> {5.0, "voltageValve8"},
        EepParameter<float> {5.0, "voltageValve9"},
        EepParameter<float> {5.0, "voltageValve10"},
        EepParameter<uint8_t> {0, "selectScale1"},
        EepParameter<uint8_t> {0, "selectScale2"},
        EepParameter<uint8_t> {0, "selectScale3"},
        EepParameter<uint8_t> {0, "selectScale4"},
        EepParameter<uint8_t> {0, "selectScale5"},
        EepParameter<uint8_t> {0, "selectScale6"},
        EepParameter<uint8_t> {0, "selectScale7"},
        EepParameter<uint8_t> {0, "selectScale8"},
        EepParameter<uint8_t> {0, "selectScale9"},
        EepParameter<uint8_t> {0, "selectScale10"},
        EepParameter<String> {"", "mqttUriValve1"},
        EepParameter<String> {"", "mqttUriValve2"},
        EepParameter<String> {"", "mqttUriValve3"},
        EepParameter<String> {"", "mqttUriValve4"},
        EepParameter<String> {"", "mqttUriValve5"},
        EepParameter<String> {"", "mqttUriValve6"},
        EepParameter<String> {"", "mqttUriValve7"},
        EepParameter<String> {"", "mqttUriValve8"},
        EepParameter<String> {"", "mqttUriValve9"},
        EepParameter<String> {"", "mqttUriValve10"},
        .useValveList = { &Settings::settingsValves.useValve1, &Settings::settingsValves.useValve2, &Settings::settingsValves.useValve3, &Settings::settingsValves.useValve4, &Settings::settingsValves.useValve5, &Settings::settingsValves.useValve6, &Settings::settingsValves.useValve7, &Settings::settingsValves.useValve8, &Settings::settingsValves.useValve9, &Settings::settingsValves.useValve10 },
        .voltageValveList = { &Settings::settingsValves.voltageValve1, &Settings::settingsValves.voltageValve2, &Settings::settingsValves.voltageValve3, &Settings::settingsValves.voltageValve4, &Settings::settingsValves.voltageValve5, &Settings::settingsValves.voltageValve6, &Settings::settingsValves.voltageValve7, &Settings::settingsValves.voltageValve8, &Settings::settingsValves.voltageValve9, &Settings::settingsValves.voltageValve10 },
        .selectScaleList = { &Settings::settingsValves.selectScale1, &Settings::settingsValves.selectScale2, &Settings::settingsValves.selectScale3, &Settings::settingsValves.selectScale4, &Settings::settingsValves.selectScale5, &Settings::settingsValves.selectScale6, &Settings::settingsValves.selectScale7, &Settings::settingsValves.selectScale8, &Settings::settingsValves.selectScale9, &Settings::settingsValves.selectScale10 },
        .mqttUriList = { &Settings::settingsValves.mqttUriValve1, &Settings::settingsValves.mqttUriValve2, &Settings::settingsValves.mqttUriValve3, &Settings::settingsValves.mqttUriValve4, &Settings::settingsValves.mqttUriValve5, &Settings::settingsValves.mqttUriValve6, &Settings::settingsValves.mqttUriValve7, &Settings::settingsValves.mqttUriValve8, &Settings::settingsValves.mqttUriValve9, &Settings::settingsValves.mqttUriValve10 }
};

RTC_DATA_ATTR EepStructScale Settings::settingsScale1 = {EepParameter<float> {0.0, "ScaleOffset1"}, EepParameter<float> {1.0, "ScaleScale1"}, EepParameter<String> {"", "ScaleMqttUri1"}, EepParameter<bool> {false, "ScaleInUse1"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale2 = {EepParameter<float> {0.0, "ScaleOffset2"}, EepParameter<float> {1.0, "ScaleScale2"}, EepParameter<String> {"", "ScaleMqttUri2"}, EepParameter<bool> {false, "ScaleInUse2"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale3 = {EepParameter<float> {0.0, "ScaleOffset3"}, EepParameter<float> {1.0, "ScaleScale3"}, EepParameter<String> {"", "ScaleMqttUri3"}, EepParameter<bool> {false, "ScaleInUse3"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale4 = {EepParameter<float> {0.0, "ScaleOffset4"}, EepParameter<float> {1.0, "ScaleScale4"}, EepParameter<String> {"", "ScaleMqttUri4"}, EepParameter<bool> {false, "ScaleInUse4"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale5 = {EepParameter<float> {0.0, "ScaleOffset5"}, EepParameter<float> {1.0, "ScaleScale5"}, EepParameter<String> {"", "ScaleMqttUri5"}, EepParameter<bool> {false, "ScaleInUse5"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale6 = {EepParameter<float> {0.0, "ScaleOffset6"}, EepParameter<float> {1.0, "ScaleScale6"}, EepParameter<String> {"", "ScaleMqttUri6"}, EepParameter<bool> {false, "ScaleInUse6"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale7 = {EepParameter<float> {0.0, "ScaleOffset7"}, EepParameter<float> {1.0, "ScaleScale7"}, EepParameter<String> {"", "ScaleMqttUri7"}, EepParameter<bool> {false, "ScaleInUse7"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale8 = {EepParameter<float> {0.0, "ScaleOffset8"}, EepParameter<float> {1.0, "ScaleScale8"}, EepParameter<String> {"", "ScaleMqttUri8"}, EepParameter<bool> {false, "ScaleInUse8"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale9 = {EepParameter<float> {0.0, "ScaleOffset9"}, EepParameter<float> {1.0, "ScaleScale9"}, EepParameter<String> {"", "ScaleMqttUri9"}, EepParameter<bool> {false, "ScaleInUse9"}};
RTC_DATA_ATTR EepStructScale Settings::settingsScale10 = {EepParameter<float> {0.0, "ScaleOffset10"}, EepParameter<float> {1.0, "ScaleScale10"}, EepParameter<String> {"", "ScaleMqttUri10"}, EepParameter<bool> {false, "ScaleInUse10"}};


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
        scaleSettings.ScaleInUse.value = this->preferences.getBool(scaleSettings.ScaleInUse.eepName, false);
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
        networkSettings.MqttName.value = this->getString(networkSettings.MqttName.eepName);
        networkSettings.UpdateInterval.value = this->getUChar(networkSettings.UpdateInterval.eepName);
    }
//    if (dataLen == sizeof(EepStructNetwork)) { // simple check that data fits
//        return nullptr; // data for scaleNr is not set, first startup!
//    }
//    auto* networkSettings = (EepStructNetwork*) buffer; // cast the bytes into a struct ptr
}

void SettingsManager::getConfigBattery() {
    if (!this->firstStartUp()) {
        Settings::settingsBattery.MqttUri.value = this->getString(Settings::settingsBattery.MqttUri.eepName);
        Settings::settingsBattery.MqttUriLow.value = this->getString(Settings::settingsBattery.MqttUriLow.eepName);
    }
}

void SettingsManager::getConfigLux() {
    if (!this->firstStartUp()) {
        Settings::settingsLux.MqttUri.value = this->getString(Settings::settingsLux.MqttUri.eepName);
    }
}

void SettingsManager::getConfigBme() {
    if (!this->firstStartUp()) {
        Settings::settingsBme.MqttUriTemp.value = this->getString(Settings::settingsBme.MqttUriTemp.eepName);
        Settings::settingsBme.MqttUriHum.value = this->getString(Settings::settingsBme.MqttUriHum.eepName);
        Settings::settingsBme.MqttUriPres.value = this->getString(Settings::settingsBme.MqttUriPres.eepName);
    }
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
    this->putString(networkSettings.MqttName);
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
    return this->preferences.getBool(eepParameter.eepName, false);
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
    log_i("SettingsManager: Loading Parameter: %s; Value: %s", eepParameter.eepName, eepParameter.value.c_str());
    return this->preferences.getString(eepParameter.eepName, "");
}

template<> void SettingsManager::saveValue(EepParameter<bool> &eepParameter) {
    log_i("SettingsManager: Saving Parameter: %s; Value: %i", eepParameter.eepName, eepParameter.value);
    this->preferences.putBool(eepParameter.eepName, eepParameter.value);
}

void SettingsManager::getConfigValves() {
    if (!this->firstStartUp()) {
        for (auto eepParameter : Settings::settingsValves.useValveList) {
            eepParameter->value = this->preferences.getBool(eepParameter->eepName, 0);
        }
        for (auto eepParameter : Settings::settingsValves.voltageValveList) {
            eepParameter->value = this->preferences.getFloat(eepParameter->eepName, 0.0);
        }
        for (auto eepParameter : Settings::settingsValves.selectScaleList) {
            eepParameter->value = this->preferences.getUChar(eepParameter->eepName, 0);
        }
        for (auto eepParameter : Settings::settingsValves.mqttUriList) {
            eepParameter->value = this->preferences.getString(eepParameter->eepName, "");
        }
    }
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
    log_i("SettingsManager: Saving Parameter: %s; Value: %s", eepParameter.eepName, eepParameter.value.c_str());
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


