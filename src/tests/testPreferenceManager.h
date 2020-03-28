#include <Arduino.h>
#include <AUnit.h>
#include "SettingsManager.h"

using namespace aunit;

aunittest(SaveAndLoadNetworkSettings) {
//    SettingsManager settingsManager = SettingsManager();
//    EepStructNetwork networkSettings = {
//            EepParameter<String> {"TestNetworkSSID", "WifiSSid"},
//            EepParameter<String> {"TestNetworkPassword", "WifiPass"},
//            EepParameter<String> {"192.168.179.56", "MqttServer"},
//            EepParameter<String> {"TestMQTTUserName", "MqttUser"},
//            EepParameter<String> {"TestMQTTPassword!§$%&/()}]?", "MqttPass"},
//            EepParameter<uint8_t> {15, "UpdateInterval"}
//    };
//    settingsManager.updateConfigNetwork(networkSettings);
//    settingsManager.getConfigNetwork(networkSettings);
//    assertTrue (networkSettings.WifiSSid.value.compareTo("TestNetworkSSID") == 0);
//    assertTrue (networkSettings.WifiPass.value.compareTo("TestNetworkPassword") == 0);
//    assertTrue (networkSettings.MqttServer.value.compareTo("192.168.179.56") == 0);
//    assertTrue (networkSettings.MqttUser.value.compareTo("TestMQTTUserName") == 0);
//    assertTrue (networkSettings.MqttPass.value.compareTo("TestMQTTPassword!§$%&/()}]?") == 0);
//    assertEqual(networkSettings.UpdateInterval.value, 15);
}

aunittest(NotExistScale10) {
    SettingsManager settingsManager = SettingsManager();
    assertFalse(settingsManager.existScale(Settings::settingsScale10));
}

aunittest(NotFirstStartup) {
    SettingsManager settingsManager = SettingsManager();
    assertFalse(settingsManager.firstStartUp());
}