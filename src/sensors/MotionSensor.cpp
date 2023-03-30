
#include "MotionSensor.h"

MotionSensor::MotionSensor() {


}

MotionSensor::MotionSensor(byte pin) {

    _pin = pin;
    init();

}

MotionSensor::MotionSensor(byte pin, const char *name, const char *synonym) {

    init(pin, name, synonym);

}

MotionSensor::~MotionSensor() {

}

byte MotionSensor::getPin() {

    return _pin;

}

const char* MotionSensor::getName() {

    return _name;

}

const char* MotionSensor::getSynonym() {

    return _synonym;

}

const char* MotionSensor::getMqttTopic() {

    return _mqtt_topic;

}

void MotionSensor::setMqttTopic(const char *mqtt_topic) {

    _mqtt_topic = mqtt_topic;

}

void MotionSensor::init(byte pin, const char *name, const char *synonym) {

    _pin = pin;
    _name = name;
    _synonym = synonym;
    init();

}

void MotionSensor::init() {

    pinMode(_pin, INPUT);

}

bool MotionSensor::getState() {

    return _state;

}

bool MotionSensor::detectsMotion() {

    _pinStatePrevious = _pinStateCurrent; // store old state
    _pinStateCurrent = digitalRead(_pin);   // read new state

    if (_pinStatePrevious == LOW && _pinStateCurrent == HIGH)  {
        _state = true;
        return true;
    } else if (_pinStatePrevious == HIGH && _pinStateCurrent == LOW) {
        _state = false;
        return false;
    }

}