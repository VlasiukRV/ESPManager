//
// Created by vlasi on 2/26/2023.
//

#ifndef ESPMANAGER_ESPCONFUGURATION_H
#define ESPMANAGER_ESPCONFUGURATION_H

#include "services/settingsManager/ESPProperties.h"
#include "sensors/SensorsManager.h"
#include "MQTTManager.h"
#include "services/WiFiConnection.h"
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"

//#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1

#define LED_RED_PIN 25
#define LED_BLUE_PIN 26

class ESPConfiguration {

public:

    ESPProperties *espProperties;
    WiFiConnection *wiFiConnection;
    AsyncWebServer *webServer;

    SensorsManager *sensorsManager;
    MQTTManager *mqttManager;

    Led *redLed;
    Led *blueLed;
    OledDisplay *oledDisplay;

    ESPConfiguration();
    bool init();
    void initESPProperties();
    bool initWiFiConnection();
    void initWebServer();
    void initSensorsManager();
    void initMQTTManager();

    void getESPPropertiesJSON(char* buffer);
    void getESPConfigurationJSON(char* buffer);
    void getESPStateJSON(char* buffer);

private:

};


#endif //ESPMANAGER_ESPCONFIGURATION_H
