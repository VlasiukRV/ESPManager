
#include <DHT.h>

#ifndef ESPMANAGER_DHTSENSOR_H
#define ESPMANAGER_DHTSENSOR_H

#define DHTTYPE DHT11

class DHTSensor {

public:

    DHTSensor();
    DHTSensor(byte pin);
    DHTSensor(byte pin, const char *name, const char *synonym);
    ~DHTSensor();

    byte getPin();
    const char* getName();
    const char* getSynonym();

    const char* getMqttTopic();
    void setMqttTopic(const char *mqtt_topic);

    void init();
    void init(byte pin, const char *name, const char *synonym);

    float readTemperature();
    char* readTemperatureStr();
    float readHumidity();
    char* readHumidityStr();

private:

    DHT *_dht;

    byte _pin;
    const char *_name;
    const char *_synonym;
    const char *_mqtt_topic;

};

#endif //ESPMANAGER_DHTSENSOR_H