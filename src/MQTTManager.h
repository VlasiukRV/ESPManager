//
// Created by vlasi on 3/11/2023.
//
#include "sensors/SensorsManager.h"
#include <WiFiClient.h>
#include <PubSubClient.h>

#ifndef ESPMANAGER_MQTTMANAGER_H
#define ESPMANAGER_MQTTMANAGER_H

#define DEVICE_ID "esp32-client-"
#define MQTT_BROKER_ADDRESS "appesp32.uptarget.co"
#define MQTT_BROKER_PORT 1083
#define MQTT_BROKER_USERNAME "admin"
#define MQTT_BROKER_PASSWORD "123456"
#define CHECK_BROKER_TIME_CONNECTION 30000

typedef std::function<void(char *topic, byte *payload, unsigned int length)> MQTTCallBackProcedure;

class MQTTManager {

public:
    MQTTManager();
    MQTTManager(char *address, int port, char *username, char *password);
    ~MQTTManager();

    void init(MQTTCallBackProcedure mqttCallBackProcedure);
    void initMQTTBroker(SensorsManager *sensorsManager);
    void initMQTTBroker(MQTTCallBackProcedure mqttCallBackProcedure);
    void connectMQTTBroker();
    void publish(const char* topic, const char* payload);
    void subscribe(const char* topic);
    void loop();
    bool connected();

    const char* device_id = DEVICE_ID;

private:
    unsigned long _previousMillis;

    WiFiClient _espClient;
    PubSubClient *_mqtt_client;

    char *_mqtt_broker_address;
    int _mqtt_broker_port;
    char *_mqtt_broker_username;
    char *_mqtt_broker_password;

};


#endif //ESPMANAGER_MQTTMANAGER_H
