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

byte Led::getPin() {
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

void Led::sendMessage(const char *message) {

    Serial.printf("Led compare [%s] [%s] [%s] \n", message, LED_MESSAGE_ON, LED_MESSAGE_OFF);

    if ( strcmp(message, LED_MESSAGE_ON) == 0) {
        on();
    }
    if ( strcmp(message, LED_MESSAGE_OFF) == 0) {
        off();
    }

}

char* Led::getState() {
    return _state;
}

void Led::setState(char* state) {
    _state = state;
    sendMessage(state);
}

void Led::on() {

    _state = LED_MESSAGE_ON;
    digitalWrite(_pin, HIGH);

}

void Led::off() {

    _state = LED_MESSAGE_OFF;
    digitalWrite(_pin, LOW);

}
