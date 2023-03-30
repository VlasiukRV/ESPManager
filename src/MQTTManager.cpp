#include <WiFiClient.h>
#include "MQTTManager.h"
#include "ESPConfiguration.h"

MQTTManager::MQTTManager(){

}

MQTTManager::~MQTTManager(){

}

MQTTManager::MQTTManager(char *address, int port, char *username, char *password) {

    _mqtt_broker_address = address;
    _mqtt_broker_port = port;
    _mqtt_broker_username = username;
    _mqtt_broker_password = password;

}

void MQTTManager::init(MQTTCallBackProcedure mqttCallBackProcedure) {

    _mqtt_client = new PubSubClient(_espClient);
    initMQTTBroker(mqttCallBackProcedure);

}

void MQTTManager::initMQTTBroker(MQTTCallBackProcedure mqttCallBackProcedure) {

    //connecting to a mqtt broker
    _mqtt_client->setServer(_mqtt_broker_address, _mqtt_broker_port);
    _mqtt_client->setCallback(mqttCallBackProcedure);

    connectMQTTBroker();
}

void MQTTManager::initMQTTBroker(SensorsManager *sensorsManager) {

    _mqtt_client = new PubSubClient(_espClient);
    _mqtt_client->setServer(_mqtt_broker_address, _mqtt_broker_port);
    _mqtt_client->setCallback([sensorsManager](char *topic, byte *payload, unsigned int length){
        String topicStr = String(topic);
        String message;
        for (int i = 0; i < length; i++) {
            message += (char)payload[i];
        }

        Serial.printf("Topic [%s] get message [%s] \n", topicStr.c_str(), message.c_str());
        sensorsManager->processingLedSensors([&topicStr, &message](Led *ledSensor) {

            Serial.printf("Led sensor topic is %s \n", ledSensor->getMqttTopic());
            if( strcmp(ledSensor->getMqttTopic(), topicStr.c_str()) == 0 ) {
                Serial.println("send message to led");
                ledSensor->sendMessage(message.c_str());
            }

        });

    });

    connectMQTTBroker();

}

void MQTTManager::connectMQTTBroker() {

    byte i = 5;
    while ( !connected() ) {

        Serial.printf("The client %s connects to the mqtt broker\n", device_id.c_str());
        if ( _mqtt_client->connect(device_id.c_str(), _mqtt_broker_username, _mqtt_broker_password) ) {

            Serial.println("mqtt broker connected");

        } else {

            Serial.printf("[%i]failed mqtt broker connected with state ", i);
            Serial.print( _mqtt_client->state() );
            delay(2000);

        }

        if ( i-- == 0 ) {
            break;
        }
    }

}

void MQTTManager::publish(const char* topic, const char* payload) {

    _mqtt_client->publish(topic, payload);

}

void MQTTManager::subscribe(const char* topic) {

    Serial.printf("subscribe [%s] \n", topic);
    _mqtt_client->subscribe(topic);

}

void MQTTManager::loop(SensorsManager *sensorsManager) {

    _mqtt_client->loop();

    sensorsManager->processingDHTSensors([&](DHTSensor *dhtSensor) {

        String temperatureTopic = String(dhtSensor->getMqttTopic()) + String("/temperature");
        publish(temperatureTopic.c_str(), dhtSensor->readTemperatureStr());
        String humidityTopic = String(dhtSensor->getMqttTopic()) + String("/humidity");
        publish(humidityTopic.c_str(), dhtSensor->readHumidityStr());

    });

    sensorsManager->processingMotionSensors([&](MotionSensor *sensor) {

        String topic = String(sensor->getMqttTopic()) + String("/motion");
        if (sensor->detectsMotion()) {
            Serial.println("Motion detected...");
            publish(topic.c_str(), "detected");
        } else {
            Serial.println("Motion stopped...");
            publish(topic.c_str(), "stopped");
        }

    });

}

bool MQTTManager::connected() {

    return _mqtt_client->connected();

}