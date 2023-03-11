//
// Created by vlasi on 2/24/2023.
//

#include "Led.h"
Led::Led() {

}

Led::Led(byte pin) {

    _pin = pin;
    init();

}

Led::Led(byte pin, char *name, char *synonym) {

    init(pin, name, synonym);

}

int Led::getPin() {
    return _pin;
}
const char* Led::getName() {
    return _name;
}
const char* Led::getSynonym() {
    return _synonym;
}
const char* Led::getMqttTopic() {
    return _mqtt_topic;
}


void Led::init(byte pin, const char *name, const char *synonym) {

    _pin = pin;
    _name = name;
    _synonym = synonym;
    init();

}

void Led::init() {

    //Serial.printf("Init Led[%s] ...pin %i\n", _name, _pin);
    pinMode(_pin, OUTPUT);
    off();

}

void Led::setMqttTopic(const char *mqtt_topic) {
    _mqtt_topic = mqtt_topic;
}

void Led::on() {

    digitalWrite(_pin, HIGH);

}

void Led::off() {

    digitalWrite(_pin, LOW);

}
