#line 2 "PlantSensor.cpp"


#define TEST_UI
//#define TEST_RUN

#include <Arduino.h>
#include "PlantSensor.h"
#include "ConfigSensorTemp.h"
#include "ConfigScale.h"
#include "ConfigBattery.h"
#include "ConfigWifi.h"
#include "WebUi.h"
#include "SettingsManager.h"
#include <Wire.h>
#include "Max44009.h"
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include "Settings.h"
#include <SPI.h>
#include <WiFi.h>
#include <nvs_flash.h>
//#ifdef TEST_RUN
//    #include <AUnit.h>
//    #include "tests/testPreferenceManager.h"
//#endif
//#include <Update.h>
//#include <AsyncTCP.h>
//#include <WiFiUdp.h>
//#include <BME280I2C.h>
//#include <HX711.h>


float batteryLevel = 0;
char batteryLevelBuffer[16] {};
float luxValue = 0;
float scaleValue;

RTC_DATA_ATTR static char bufferWifiSSid[32] = "";
RTC_DATA_ATTR static char bufferWifiPass[64] = "";
RTC_DATA_ATTR static char bufferMqttServer[64] = "";
RTC_DATA_ATTR static char bufferMqttUser[64] = "";
RTC_DATA_ATTR static char bufferMqttPass[64] = "";
RTC_DATA_ATTR static char bufferMqttName[64] = "";
// MQTT URIs
RTC_DATA_ATTR static char bufferMqttUriBattery[128] = "";
RTC_DATA_ATTR static char bufferMqttUriBatteryLow[128] = "";
RTC_DATA_ATTR static char bufferMqttUriTemp[128] = "";
RTC_DATA_ATTR static char bufferMqttUriHum[128] = "";
RTC_DATA_ATTR static char bufferMqttUriPres[128] = "";
RTC_DATA_ATTR static char bufferMqttUriLux[128] = "";


Max44009 SensorLux(0x4A, 4, 5);
#define luxMid 5
char luxBuffer[16];

RTC_DATA_ATTR bool firstStart = true;

//#define mqtt_topics_len 6
//char const* mqtt_topics[mqtt_topics_len] = { "/home/balkon/temp/", "/home/balkon/hum/", "/home/balkon/pres/", "/home/balkon/lux/", "/home/balkon/batteryLevel/", "/home/balkon/haengekasten_rechts/" };

uint16_t start_time = millis();
uint16_t volatile current_time = millis();
IPAddress mqttServerAddress;

void setupScales_(EepStructScale &eepStructScale, Scale &scale) {
    scale.scale = eepStructScale.ScaleScale.value;
    scale.offset = eepStructScale.ScaleOffset.value;
    scale.mqttUri = eepStructScale.ScaleMqttUri.value.c_str();
}

void setupScales() {
    setupScales_(Settings::settingsScale1, Scale1);
    setupScales_(Settings::settingsScale2, Scale2);
    setupScales_(Settings::settingsScale3, Scale3);
    setupScales_(Settings::settingsScale4, Scale4);
    setupScales_(Settings::settingsScale5, Scale5);
    setupScales_(Settings::settingsScale6, Scale6);
    setupScales_(Settings::settingsScale7, Scale7);
    setupScales_(Settings::settingsScale8, Scale8);
    setupScales_(Settings::settingsScale9, Scale9);
    setupScales_(Settings::settingsScale10, Scale10);
}

void setup() {
    pinMode(GPIO_Battery_READ, INPUT);
    log_i("Starting");
    #if defined(DEBUG) || defined(TEST_RUN) || defined(TEST_UI)
    Serial.begin(115200);
    #endif
	/*WiFi.disconnect();
	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();
	delay(1);
	WiFi.forceSleepWake();
	WiFi.mode(WIFI_STA);
	wifi_station_connect();
	*/
#ifndef TEST_RUN
    SettingsManager settingsManager = SettingsManager();
#ifdef TEST_UI
    setup_espui(settingsManager);
    return;
#endif
    if (firstStart && settingsManager.firstStartUp()) setup_espui(settingsManager); // is first start up ever?
    if (firstStart) { // is not startup after deep sleep?
        settingsManager.getConfigNetwork();
        settingsManager.getConfigBattery();
        settingsManager.getConfigBme();
        settingsManager.getConfigLux();
        strcpy(bufferWifiSSid, Settings::settingsNetwork.WifiSSid.value.c_str());
        strcpy(bufferWifiPass, Settings::settingsNetwork.WifiPass.value.c_str());
        strcpy(bufferMqttServer, Settings::settingsNetwork.MqttServer.value.c_str());
        strcpy(bufferMqttUser, Settings::settingsNetwork.MqttUser.value.c_str());
        strcpy(bufferMqttPass, Settings::settingsNetwork.MqttPass.value.c_str());
        strcpy(bufferMqttName, Settings::settingsNetwork.MqttName.value.c_str());
        strcpy(bufferMqttUriBattery, Settings::settingsBattery.MqttUri.value.c_str());
        strcpy(bufferMqttUriTemp, Settings::settingsBme.MqttUriTemp.value.c_str());
        strcpy(bufferMqttUriHum, Settings::settingsBme.MqttUriHum.value.c_str());
        strcpy(bufferMqttUriPres, Settings::settingsBme.MqttUriPres.value.c_str());
//        strcpy(bufferMqttUriScale1, Settings::settingsScale1.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale2, Settings::settingsScale2.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale3, Settings::settingsScale3.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale4, Settings::settingsScale4.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale5, Settings::settingsScale5.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale6, Settings::settingsScale6.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale7, Settings::settingsScale7.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale8, Settings::settingsScale8.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale9, Settings::settingsScale9.ScaleMqttUri.value.c_str());
//        strcpy(bufferMqttUriScale10, Settings::settingsScale10.ScaleMqttUri.value.c_str());
        settingsManager.getConfigScalesAll();
        setupScales();
        firstStart = false;
    }
    log_d("Starting normal routine");
    WiFi.mode(WIFI_STA);
	WiFi.begin(bufferWifiSSid, bufferWifiPass);
	WiFi.persistent(false);
	log_d("Wifi started: %s, %s", bufferWifiSSid, bufferWifiPass);
//	  WiFi.mode(WIFI_OFF);
//	  WiFi.mode(WIFI_STA);
//    WiFi.begin(Settings::settingsNetwork.WifiSSid.value.c_str(), Settings::settingsNetwork.WifiPass.value.c_str());
    log_d("Setting mqtt server: %s Port: %i", bufferMqttServer, Settings::settingsNetwork.MqttPort.value);

    mqttServerAddress.fromString(bufferMqttServer);
    myMqttClient.setServer(mqttServerAddress, 1883);
    log_d("Server IP is: %d", mqttServerAddress.toString().c_str());

	//int volatile * const p_reg = (int *) 0x60000808;
	//*p_reg = 0b000010010 | GPOS;
	//*p_reg = 0b000010010 | GPOS;
	//digitalWrite(12, HIGH);
	//digitalWrite(13, HIGH);
	//delay(3000);

    readBatteryLevel(8);
	readLux();
//    readScales();
	readBME();

	volatile bool connected = false;
	current_time = millis();
	start_time = millis();
	volatile uint8_t retries = 0;
	log_d("Connecting to network...");
	while (connected == false && retries <= 2) {
		connected = reconnect();
		delay(100);
		log_d("Number of retries: %i", retries);
		retries++;
	}

	if (connected) {
		//Publish all values:
		//TODO Add Timeout 
        myMqttClient.publish(bufferMqttUriBattery, batteryLevelBuffer);
        myMqttClient.publish(bufferMqttUriBatteryLow, "0");
		for (int i = 0; i < NrOfScales; i++) {
            myMqttClient.publish(Scales[i]->mqttUri, Scales[i]->scaleValueBuffer);
			log_d("Publishing Scale Value: %s", Scales[i]->scaleValueBuffer);
		}
        myMqttClient.publish(bufferMqttUriLux, luxBuffer);
        myMqttClient.publish(bufferMqttUriTemp, tempValueBuffer);
        myMqttClient.publish(bufferMqttUriHum, humValueBuffer);
        myMqttClient.publish(bufferMqttUriPres, presValueBuffer);
		log_d("Publishing Battery Level: %s, going to sleep...", batteryLevelBuffer);
	}

	if (digitalRead(0) == 0) {
        delay(50);
        if (digitalRead(0) == 0) debug_menu();
    }

	//DeepSleep:
	if (checkBatteryLevelWarning()) {
        readBatteryLevel(8);
        if (checkBatteryLevelWarning()) {
		    log_d("Low Voltage detected: %f", getBatteryVoltage());
            myMqttClient.publish(bufferMqttUriBatteryLow, "1");
            if (checkBatteryLevelLow()) {
                digitalWrite(SCALE_SCK, HIGH);
                waitUntilPublished();
                ESP.deepSleep(0);
            }
		}
	}

    waitUntilPublished();

	ESP.deepSleep(UPDATE_INTERVAL);
#endif
}

inline void waitUntilPublished() {
    myMqttClient.disconnect();
    myEspClient.flush();
    while(myMqttClient.state() != -1)
        delay(10);
}

void loop()
{

#ifdef TEST_RUN
    aunit::TestRunner::run();
#endif
#ifndef TEST_RUN
	ESP.deepSleep(UPDATE_INTERVAL); // if loop is reached somehow?
#endif
}

void callback(char* topic, byte* payload, unsigned int length) {}

inline boolean checkBatteryLevelLow() {
	return (getBatteryVoltage() < SHUTDOWN_VOLTAGE);
}

inline bool checkBatteryLevelWarning() {
    return (getBatteryVoltage() < SHUTDOWN_VOLTAGE + 0.15);
}

void inline readScales() {
    log_d("Begin reading scales...");
	for (int i = 0; i < NrOfScales; i++) {
		Scales[i]->main->begin(Scales[i]->dOut, Scales[i]->sck);
		Scales[i]->main->set_gain(Scales[i]->gain);
		scaleValue = (Scales[i]->main->read_average(3) - Scales[i]->offset) / Scales[i]->scale;
		snprintf(Scales[i]->scaleValueBuffer, sizeof(Scales[i]->scaleValueBuffer), "%.2f", scaleValue);
        log_d("Scale Nr.: %i; Value: %f", i+1, scaleValue);
	}
    digitalWrite(SCALE_SCK, HIGH);	// set scales to sleep
}

void inline readLux() {
    luxValue = 0;
	if (SensorLux.getError() == 0) {
		for (int i = 1; i < luxMid; i++) {
            luxValue += SensorLux.getLux();
			delay(5);
		}
        luxValue = luxValue / luxMid;
		snprintf(luxBuffer, sizeof(luxBuffer), "%.1f", luxValue);
        log_d("Updating Lux: %f", luxValue);
	}
}

void inline readBME() {
    log_d("Begin reading BME...");
	startBME();
	SensorBme.read(pres, temp, hum, tempUnit, presUnit);
	// Pressure Exception:
	uint8_t retries = 0;
	while (pres <= 0.65 && retries <= 5) {
		startBME();
		SensorBme.read(pres, temp, hum, tempUnit, presUnit);
		retries++;
	}
	snprintf(tempValueBuffer, sizeof(tempValueBuffer), "%.1f", temp);
	snprintf(humValueBuffer, sizeof(humValueBuffer), "%.1f", hum);
	snprintf(presValueBuffer, sizeof(presValueBuffer), "%.4f", pres);
	log_d("Updating BME: T=%s, H=%s, P=%s", tempValueBuffer, humValueBuffer, presValueBuffer);
}

void inline startBME() { if (!SensorBme.begin()) SensorBme.begin(); }

void debug_menu()
{
	Serial.begin(115200);
	pinMode(0, OUTPUT);
	delay(10);
	digitalWrite(0, HIGH);
	start_ota();
	calibrate_scales();
}

void start_ota()
{
	ArduinoOTA.onStart([]() {
		log_d("Starting Arduino OTA");
	});
	ArduinoOTA.onEnd([]() {
        log_d("Ending Arduino OTA");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        log_d("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		log_e("ArduinoOta Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) log_e("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) log_e("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) log_e("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) log_e("Receive Failed");
		else if (error == OTA_END_ERROR) log_e("End Failed");
	});
	ArduinoOTA.begin();
}

void calibrate_scales() {
	Serial.println("");
	Serial.println("Starting calibration.");
	Serial.println("Available commands:");
	Serial.println("get <scaleNr>");
	Serial.println("set <scaleNr> <offset>");
	Serial.println("gain <scaleNr> <gain>");
	Serial.println("tar <scaleNr>");
	Serial.println("stop");
	Serial.println("");
	Serial.println("Type start or any other command within 10s to begin!");
	for (int i = 0; i < NrOfScales; i++) {
		Scales[i]->main->begin(Scales[i]->dOut, Scales[i]->sck);
		Scales[i]->main->set_gain(128);
		Scales[i]->main->set_scale();
	}
	const int start_time = millis();
	bool started = false;
	while (start_time + 10*1000 < millis() || started) {
		ArduinoOTA.handle();
		if (Serial.available()) {
			String input = Serial.readString();
			//Serial.print("You typed: " );
			//Serial.println(input);
			int index = input.indexOf(" ");
			if (index <= 0) index = 3;
			String cmd = input.substring(0, index);
			String params = input.substring(index + 1);
			int scaleNr = params.substring(0, params.indexOf(" ")).toInt();
			float cmdValue = params.substring(params.indexOf(" ") + 1).toInt();
			/*Serial.print("cmd: "); Serial.println(cmd);
			Serial.print("params: "); Serial.println(params);
			Serial.print("scaleNr: "); Serial.println(scaleNr);
			Serial.print("cmdValue: "); Serial.println(cmdValue);*/
			if (cmd == "get") {
				started = true;
				Serial.print("Value from scale "); Serial.print(scaleNr); Serial.print(" : ");
				Serial.println(Scales[scaleNr]->main->get_units(30));
			}
			if (cmd == "set") {
				started = true;
				Serial.print("Setting scale "); Serial.print(scaleNr); Serial.print(" to: "); Serial.println(cmdValue);
				Scales[scaleNr]->main->set_scale(cmdValue);
			}
			if (cmd == "tar") {
				started = true;
				Serial.print("Taring scale: "); Serial.print(scaleNr);
				Scales[scaleNr]->main->set_scale();
				Scales[scaleNr]->main->tare();
				Serial.println(" - Finished.");
			}
			if (cmd == "gain") {
				started = true;
				Serial.print("Setting scale "); Serial.print(scaleNr); Serial.print(" gain to: ");
				Serial.println(cmdValue);
				Scales[scaleNr]->main->set_gain(int(cmdValue));
			}
			if (cmd == "stop") {
				started = true;
				Serial.print("Finished calibrating. Restarting...");
				ESP.restart();
			}
			if (cmd == "start") {
				started = true;
				Serial.print("Starting.");

			}
		}
	}
}

// attempt to connect to the wifi if connection is lost
bool reconnect() {
	if (WiFi.status() != WL_CONNECTED) {
		log_d("Connecting to Wifi Network: %s", bufferWifiSSid);
	    start_time = millis();
		current_time = millis();
		while (WiFi.status() != WL_CONNECTED && start_time + 15*1000 > current_time) {
			delay(500);
			current_time = millis();
		}
		if (WiFi.status() == WL_CONNECTED) {
            log_d("Wifi connected. IP Address: %s", WiFi.localIP().toString().c_str());
		}
		else{
			log_d("Timed out while connecting to Wifi...");
		}
	}
	// make sure we are connected to WIFI before attempting to reconnect to MQTT
	if (WiFi.status() == WL_CONNECTED) {
		start_time = millis();
		current_time = millis();
		volatile uint8_t retries = 0;
		while (!myMqttClient.connected() && (retries <= 3)) {
			retries++;
			log_d("Attempting MQTT connection: User: %s, Password: %s", bufferMqttUser, bufferMqttPass);
			// Attempt to connect to MQTT Server
			if (myMqttClient.connect(bufferMqttName, bufferMqttUser, bufferMqttPass)) {
			    log_d("Connected to Mqtt Server.");
			}
			else {
			    log_d("Failed to connect to Mqtt Server; State: %i; trying again in 1s...", myMqttClient.state());
				delay(1000);  // Wait 1 seconds before retrying
			}
		}
	}
	if ((WiFi.status() == WL_CONNECTED) && myMqttClient.connected()) {
		return true;
	}
	else {
		return false;
	}
}
/*
//generate unique name from MAC addr
String macToStr(const uint8_t* mac) {
	String result;
	for (int i = 0; i < 6; ++i) {
		result += String(mac[i], 16);

		if (i < 5) {
			result += ':';
		}
	}
	return result;
}*/