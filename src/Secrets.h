#ifndef _SECRETS_H
#define _SECRETS_H

#include <Arduino.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define SECRET_WIFI_SSID	""
#define SECRET_WIFI_PASS	""

#define SECRET_MQTT_SERVER	""  //your MQTT IP Address
#define SECRET_MQTT_USER	""
#define SECRET_MQTT_PASS	""

#endif