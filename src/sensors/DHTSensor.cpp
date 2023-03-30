
#include "DHTSensor.h"

DHTSensor::DHTSensor() {

}

DHTSensor::DHTSensor(byte pin) {

    _pin = pin;
    init();

}

DHTSensor::DHTSensor(byte pin, const char *name, const char *synonym) {

    init(pin, name, synonym);

}

DHTSensor::~DHTSensor() {

}

byte DHTSensor::getPin() {

    return _pin;

}

const char* DHTSensor::getName() {

    return _name;

}

const char* DHTSensor::getSynonym() {

    return _synonym;

}

const char* DHTSensor::getMqttTopic() {

    return _mqtt_topic;

}

void DHTSensor::setMqttTopic(const char *mqtt_topic) {

    _mqtt_topic = mqtt_topic;

}

void DHTSensor::init(byte pin, const char *name, const char *synonym) {

    _pin = pin;
    _name = name;
    _synonym = synonym;
    init();

}

void DHTSensor::init() {

    _dht = new DHT(_pin, DHTTYPE);
    _dht->begin();

}

float DHTSensor::readTemperature() {

    return _dht->readTemperature(true);

}

char* DHTSensor::readTemperatureStr() {

    char* buf = new char[6];
    buf[5] = '\0';

    dtostrf(readTemperature(), 6, 2, buf);

    return &buf[0];
}

float DHTSensor::readHumidity() {

    return _dht->readHumidity();

}

char* DHTSensor::readHumidityStr() {

    char* buf = new char[6];
    buf[5] = '\0';

    dtostrf(readHumidity(), 6, 2, buf);

    return &buf[0];

}
