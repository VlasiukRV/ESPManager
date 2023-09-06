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
    _state = "";
    pinMode(_pin, OUTPUT);
    off();

}

void Led::setMqttTopic(const char *mqtt_topic) {
    _mqtt_topic = mqtt_topic;
}

void Led::sendMessage(const char *message) {

    Serial.printf("Led compare [%s] [%s] [%s] \n", message, LED_MESSAGE_ON, LED_MESSAGE_OFF);

    if ( strcmp(message, LED_MESSAGE_ON) == 0) {
        _state = LED_MESSAGE_ON;
        on();
    } else if ( strcmp(message, LED_MESSAGE_OFF) == 0) {
        _state = LED_MESSAGE_OFF;
        off();
    }else if ( strcmp(message, LED_MESSAGE_BLINK) == 0) {
        _state = LED_MESSAGE_BLINK;
        blink();
    } else {
        _state = "";
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

    _ledState = HIGH;
    digitalWrite(_pin, _ledState);

}

void Led::off() {

    _ledState = LOW;
    digitalWrite(_pin, _ledState);

}

void Led::blink() {

    if ( strcmp(_state, LED_MESSAGE_BLINK) != 0){
        return;
    }

    unsigned long currentMillis = millis();

    if (currentMillis - _previousMillis >= LED_BLINK_INTERVAL) {
        // save the last time you blinked the LED
        _previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        if (_ledState == LOW) {
            on();
        } else {
            off();
        }

    }

}

void Led::loop() {

    blink();

}
