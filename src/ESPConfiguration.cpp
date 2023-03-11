
#include "ESPConfiguration.h"

ESPConfiguration::ESPConfiguration() {

}

bool ESPConfiguration::init() {

    redLed = new Led(LED_RED_PIN);
    redLed->off();

    blueLed = new Led(LED_BLUE_PIN);
    blueLed->off();

    oledDisplay = new OledDisplay(1);
    delay(2000);
    //oledDisplay->drawBitmap(epd_bitmap_allArray[0]);
    oledDisplay->printMessage("Hellow World!");

    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println(F("An Error has occurred while mounting SPIFFS"));
        oledDisplay->printMessage("An Error has occurred while mounting SPIFFS");
        return false;
    }

    initESPProperties();
    if ( initWiFiConnection() ) {

        initWebServer();
        initSensorsManager();

        return true;
    }

    return false;

}

void ESPConfiguration::initSensorsManager() {

    sensorsManager = new SensorsManager();

}

void ESPConfiguration::initESPProperties() {

    espProperties = new ESPProperties();

    espProperties->addSetting("ssid", "WIFI SSID:", "VRnet");
    espProperties->addSetting("password", "WIFI PASSWORD:", "a m e n !");
    Serial.println(F(" - load ESP settings"));

    if (espProperties->loadSettings()) {
        Serial.println(F("  - settings loaded:"));
    }

}

bool ESPConfiguration::initWiFiConnection() {

    wiFiConnection = new WiFiConnection();

    wiFiConnection->setWiFiProperties(espProperties->getSettingStr("ssid"),
                                      espProperties->getSettingStr("password"));
    if (wiFiConnection->init()) {

        if (wiFiConnection->isWiFiConnected()) {
            blueLed->on();
            oledDisplay->printMessage("Connected to WiFi");
        }
        if (wiFiConnection->isAccessPoint()) {
            redLed->on();
            oledDisplay->printMessage("Access point started");
        }

        return true;
    }

    return false;
}

void ESPConfiguration::initWebServer() {

    //AsyncWebServer server(80);
    webServer = new AsyncWebServer(80);

    webServer->onNotFound([](AsyncWebServerRequest *request){
        Serial.println(".... onNotFound");
        request->send(404, "text/plain", "Not found");
    });

    webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println(".... HTTP_GET /");
        request->send(SPIFFS, "/index.html");
    });

    webServer->on("/bootstrap.css", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println(".... HTTP_GET /style.css");
        request->send(SPIFFS, "/bootstrap.css", "text/css");
    });
    webServer->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println(".... HTTP_GET /style.css");
        request->send(SPIFFS, "/style.css", "text/css");
    });
    webServer->on("/jquery.js", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println(".... HTTP_GET /jquery.js");
        request->send(SPIFFS, "/jquery.js", "text/javascript");
    });
    webServer->on("/bootstrap.js", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println(".... HTTP_GET /bootstrap.js");
        request->send(SPIFFS, "/bootstrap.js", "text/javascript");
    });
    webServer->on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println(".... HTTP_GET /main.js");
        request->send(SPIFFS, "/main.js", "text/javascript");
    });

    webServer->on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
        Serial.println(".... HTTP_POST /reset");
        request->send(200,"text/plain","OK");
        ESP.restart();
    });

    webServer->begin();
    delay(2000);

    Serial.println("HTTP server started");

}

void ESPConfiguration::getESPPropertiesJSON(char* buffer) {

    DynamicJsonDocument doc(1024);

    JsonArray jsonSettings = doc.createNestedArray("settings");

    espProperties->processingProperties([&jsonSettings](ESPProperty *property){
        JsonObject obj = jsonSettings.createNestedObject();
        obj["name"] = property->getName();
        obj["synonym"] = property->getSynonym();
        obj["type"] = property->getType();
        switch (property->getType()) {
            case CHAR:
                obj["value"] = property->getValueStr();
                break;
            case INT:
                obj["value"] = property->getValueInt();
                break;
            case LONG:
                obj["value"] = property->getValueLong();
                break;
        }
    });

    JsonObject jsonESPProperties = doc.createNestedObject("espProperties");
    jsonESPProperties["name"] = "hub";
    jsonESPProperties["synonym"] = "Main hub";

    JsonArray jsonSensors = doc.createNestedArray("sensors");
    sensorsManager->processingLedSensors([&jsonSensors](Led *ledSensor) {
        JsonObject sensorObject = jsonSensors.createNestedObject();
        sensorObject["name"] = ledSensor->getName();
        sensorObject["synonym"] = ledSensor->getSynonym();
        sensorObject["type"] = "led";
        JsonObject sensorProperties = jsonSensors.createNestedObject("properties");
        sensorProperties["pin"] = ledSensor->getPin();
        sensorProperties["mqtt_topic"] = ledSensor->getMqttTopic();
    });

    serializeJson(doc, buffer, 512);

}

void ESPConfiguration::getESPStateJSON(char* buffer) {

    DynamicJsonDocument doc(1024);

    doc["heap"] = ESP.getFreeHeap();

    serializeJson(doc, buffer, 512);

}