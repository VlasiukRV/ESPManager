
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
        initMQTTManager();
        initESPConfiguration();
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
    espProperties->addSetting("password", "WIFI PASSWORD:", "");
    espProperties->addSetting("mqttAddress", "MQTT BROKER ADDRESS:", "");
    espProperties->addSetting("mqttPort", "MQTT BROKER PORT:", 1083);
    espProperties->addSetting("mqttUsername", "MQTT BROKER USERNAME:", "");
    espProperties->addSetting("mqttPassword", "MQTT BROKER PASSWORD:", "");

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

    serializeJson(doc, buffer, 512);

}

void ESPConfiguration::setESPConfigurationJSON(DynamicJsonDocument* doc) {

    Serial.println("prepare JSON configuration");
    JsonArray jsonSensors = doc->createNestedArray("sensors");

    sensorsManager->processingLedSensors([&jsonSensors](Led *ledSensor) {
        Serial.println("prepare JSON configuration (Led)");

        JsonObject sensorObject = jsonSensors.createNestedObject();
        sensorObject["name"] = ledSensor->getName();
        sensorObject["synonym"] = ledSensor->getSynonym();
        sensorObject["type"] = "led";

        JsonObject sensorProperties = sensorObject.createNestedObject("properties");
        sensorProperties["pin"] = ledSensor->getPin();
        sensorProperties["mqtt_topic"] = ledSensor->getMqttTopic();

        JsonObject sensorValue = sensorObject.createNestedObject("value");
        sensorValue["state"] = ledSensor->getState();
    });

    sensorsManager->processingDHTSensors([&jsonSensors](DHTSensor *dhtSensor) {

        Serial.println("prepare JSON configuration (DHT)");

        JsonObject sensorObject = jsonSensors.createNestedObject();
        sensorObject["name"] = dhtSensor->getName();
        sensorObject["synonym"] = dhtSensor->getSynonym();
        sensorObject["type"] = "dht11";

        JsonObject sensorProperties = sensorObject.createNestedObject("properties");
        sensorProperties["pin"] = dhtSensor->getPin();
        sensorProperties["mqtt_topic"] = dhtSensor->getMqttTopic();

        JsonObject sensorValue = sensorObject.createNestedObject("value");
        sensorValue["temperature"] = dhtSensor->readTemperatureStr();
        sensorValue["humidity"] = dhtSensor->readHumidityStr();
    });

    sensorsManager->processingMotionSensors([&jsonSensors](MotionSensor *motionSensor) {

        Serial.println("prepare JSON configuration (MotionSensor)");

        JsonObject sensorObject = jsonSensors.createNestedObject();
        sensorObject["name"] = motionSensor->getName();
        sensorObject["synonym"] = motionSensor->getSynonym();
        sensorObject["type"] = "motion";

        JsonObject sensorProperties = sensorObject.createNestedObject("properties");
        sensorProperties["pin"] = motionSensor->getPin();
        sensorProperties["mqtt_topic"] = motionSensor->getMqttTopic();

        JsonObject sensorValue = sensorObject.createNestedObject("value");
        sensorValue["state"] = motionSensor->getState();

    });

}

void ESPConfiguration::initESPConfiguration() {

    bool loadESPConfiguration = false;

    DynamicJsonDocument doc(1024);

    Serial.println("Load configurationJSON:");
    File file = SPIFFS.open("/configuration.json", FILE_READ);
    if (file && file.size()) {
        int size = file.size();
        Serial.printf("read file %i \n", size);
        DeserializationError error = deserializeJson(doc, file);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            loadESPConfiguration = false;
        } else {
            Serial.print(F("deserializeJson() ok: "));
            loadESPConfiguration = true;
        }

    }
    file.close();

    //if (espProperties->loadESPConfiguration(doc)) {
    if (loadESPConfiguration) {
        Serial.println(F("  - configuration loaded:"));

        JsonArray jsonSensors = doc["sensors"].as<JsonArray>();
        for(JsonVariant sensor : jsonSensors) {

            const char* type = sensor["type"].as<const char*>();
            String typeStr = String(type);

            const char *name = sensor["name"].as<const char*>();
            const char *synonym = sensor["synonym"].as<const char*>();
            int pin = sensor["properties"]["pin"].as<int>();
            const char *mqtt_topic = sensor["properties"]["mqtt_topic"].as<const char*>();

            if( strcmp("led", typeStr.c_str() ) == 0 ) {
                Serial.println(F("          - loaded:led"));

                sensorsManager->addLedSensor( pin, strdup(name), strdup(synonym), strdup(mqtt_topic) );
                mqttManager->subscribe(strdup(mqtt_topic));
            } else if ( strcmp("dht11", typeStr.c_str() ) == 0 ) {
                Serial.println(F("          - loaded:dht11"));

                sensorsManager->addDHTSensor( pin, strdup(name), strdup(synonym), strdup(mqtt_topic) );
            } else if ( strcmp("motion", typeStr.c_str() ) == 0 ) {
                Serial.println(F("          - loaded:motion"));

                sensorsManager->addMotionSensor( pin, strdup(name), strdup(synonym), strdup(mqtt_topic) );
            }
        }

    }

}

void ESPConfiguration::getESPConfigurationJSON(char* buffer) {

    DynamicJsonDocument doc(1024);

    setESPConfigurationJSON(&doc);
    serializeJson(doc, buffer, 512);

}

void ESPConfiguration::getESPStateJSON(char* buffer) {

    DynamicJsonDocument doc(1024);

    doc["cpuMHz"] = ESP.getCpuFreqMHz();

    doc["heap"] = ESP.getFreeHeap();
    doc["maxHeap"] = ESP.getMaxAllocHeap();

    doc["flashSpeedHz"] = ESP.getFlashChipSpeed();
    doc["flashChipSize"] = ESP.getFlashChipSize();
    doc["SketchSize"] = ESP.getSketchSize();
    //doc["Vcc"] = ESP.getVcc();

    serializeJson(doc, buffer, 512);

}

void ESPConfiguration::initMQTTManager(){

    mqttManager = new MQTTManager(espProperties->getSettingStr("mqttAddress"),
                                  espProperties->getSettingInt("mqttPort"),
                                  espProperties->getSettingStr("mqttUsername"),
                                  espProperties->getSettingStr("mqttPassword"));

    mqttManager->initMQTTBroker(sensorsManager);

}

void ESPConfiguration::loop() {

    wiFiConnection->loop();

    if (!mqttManager->connected()) {
        //mqttManager->connectMQTTBroker();
    } else {

        mqttManager->loop();

        Serial.print("..... begin ");
        Serial.println(ESP.getFreeHeap());
        sensorsManager->processingDHTSensors([&](DHTSensor *dhtSensor) {

            const char* topic_prefix = dhtSensor->getMqttTopic();
            const char* temperature_postfix = "/temperature";
            const char* humidity_postfix = "/humidity";

            int size = strlen(topic_prefix) + strlen(temperature_postfix) + 1;
            char* temperature_topic = new char[size];
            strcpy(temperature_topic,topic_prefix);
            strcat(temperature_topic,temperature_postfix);
            //const char* temperatureTopic_char = (const char*) temperature_topic;

            mqttManager->publish((const char*) temperature_topic, dhtSensor->readTemperatureStr());

            size = strlen(topic_prefix) + strlen(humidity_postfix) + 1;
            char* humidity_topic = new char[size];
            strcpy(humidity_topic,topic_prefix);
            strcat(humidity_topic,humidity_postfix);
            //const char* humidityTopic_char = (const char*) humidity_topic;

            mqttManager->publish((const char*) humidity_topic, dhtSensor->readHumidityStr());

            delete[] temperature_topic;
            delete[] humidity_topic;

        });
        Serial.print("..... end ");
        Serial.println(ESP.getFreeHeap());

        sensorsManager->processingMotionSensors([&](MotionSensor *sensor) {

            const char* topic_prefix = sensor->getMqttTopic();
            const char* topic_postfix = "/motion";

            int size = strlen(topic_prefix) + strlen(topic_postfix) + 1;
            char* topic = new char[size];
            strcpy(topic,topic_prefix);
            strcat(topic,topic_postfix);
            //const char* topic_char = (const char*) topic;

            if (sensor->detectsMotion()) {
                Serial.println("Motion detected...");
                mqttManager->publish((const char*) topic, "detected");
            } else {
                Serial.println("Motion stopped...");
                mqttManager->publish((const char*) topic, "stopped");
            }

            delete[] topic;
        });

    }

    sensorsManager->processingLedSensors([](Led *ledSensor) {
        ledSensor->loop();
    });

}