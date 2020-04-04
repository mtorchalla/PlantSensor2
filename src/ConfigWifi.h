#ifndef PLANTSENSOR_CONFIGWIFI_H
#define PLANTSENSOR_CONFIGWIFI_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "WiFiClient.h"

//WIFI:
//const char* ssid = WIFI_SSID;
//const char* password = WIFI_PASS;
//const char* mqttUser = MQTT_USER;
//const char* mqttPassword = MQTT_PASS;
WiFiClient myEspClient;
PubSubClient myMqttClient(myEspClient);
//PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

#endif //PLANTSENSOR_CONFIGWIFI_H
