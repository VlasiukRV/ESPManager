
#include "SensorsManager.h"

SensorsManager::SensorsManager() {

    _ledSensorsCounter = 0;
    _dhtSensorsCounter = 0;
    _motionSensorsCounter = 0;
}

SensorsManager::~SensorsManager() {

}

void SensorsManager::addLedSensor(int pin, const char *name, const char *synonym) {

    if (_ledSensorsCounter < _ledSensorsLengthMax) {
        _ledSensorsContainer[_ledSensorsCounter].init(pin, name, synonym);
        _ledSensorsCounter++;
    }

}

void SensorsManager::addLedSensor(int pin, const char *name, const char *synonym, const char *mqtt_topic) {

    if (_ledSensorsCounter < _ledSensorsLengthMax) {
        _ledSensorsContainer[_ledSensorsCounter].init(pin, name, synonym);
        _ledSensorsContainer[_ledSensorsCounter].setMqttTopic(mqtt_topic);
        Serial.printf("Add Led [%s] use message [%s] \n", name, mqtt_topic);
        _ledSensorsCounter++;
    }

}

void SensorsManager::processingLedSensors(ProcessLedSensor Procedure) {

    for (int i = 0; i < _ledSensorsCounter; i++) {
        Procedure(&this->_ledSensorsContainer[i]);
    }

}

void SensorsManager::addDHTSensor(int pin, const char *name, const char *synonym) {

    if (_dhtSensorsCounter < _dhtSensorsLengthMax) {
        _dhtSensorsContainer[_dhtSensorsCounter].init(pin, name, synonym);
        _dhtSensorsCounter++;
    }

}

void SensorsManager::addDHTSensor(int pin, const char *name, const char *synonym, const char *mqtt_topic) {

    if (_dhtSensorsCounter < _dhtSensorsLengthMax) {
        _dhtSensorsContainer[_dhtSensorsCounter].init(pin, name, synonym);
        _dhtSensorsContainer[_dhtSensorsCounter].setMqttTopic(mqtt_topic);
        Serial.printf("Add DHT Sensor [%s] use message [%s] \n", name, mqtt_topic);
        _dhtSensorsCounter++;
    }

}

void SensorsManager::processingDHTSensors(ProcessDHTSensor Procedure) {

    for (int i = 0; i < _dhtSensorsCounter; i++) {
        Procedure(&this->_dhtSensorsContainer[i]);
    }

}
void SensorsManager::addMotionSensor(int pin, const char *name, const char *synonym) {

    if (_motionSensorsCounter < _motionSensorsLengthMax) {
        _motionSensorsContainer[_motionSensorsCounter].init(pin, name, synonym);
        _motionSensorsCounter++;
    }

}

void SensorsManager::addMotionSensor(int pin, const char *name, const char *synonym, const char *mqtt_topic) {

    if (_motionSensorsCounter < _motionSensorsLengthMax) {
        _motionSensorsContainer[_motionSensorsCounter].init(pin, name, synonym);
        _motionSensorsContainer[_motionSensorsCounter].setMqttTopic(mqtt_topic);
        Serial.printf("Add Motion Sensor [%s] use message [%s] \n", name, mqtt_topic);
        _motionSensorsCounter++;
    }

}

void SensorsManager::processingMotionSensors(ProcessMotionSensor Procedure) {

    for (int i = 0; i < _motionSensorsCounter; i++) {
        Procedure(&this->_motionSensorsContainer[i]);
    }

}
