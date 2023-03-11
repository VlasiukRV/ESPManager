//
// Created by vlasi on 2/24/2023.
//
#include <Arduino.h>

#ifndef ESPMANAGER_LED_H
#define ESPMANAGER_LED_H

class Led {

public:
    Led();
    Led(byte pin);
    Led(byte pin, char *name, char *synonym);

    int getPin();
    const char* getName();
    const char* getSynonym();
    const char* getMqttTopic();

    void setMqttTopic(const char *mqtt_topic);

    void init(byte pin, const char *name, const char *synonym);
    void init();

    void on();
    void off();

private:

    byte _pin;
    const char *_name;
    const char *_synonym;
    const char *_mqtt_topic;
};

#endif //ESPMANAGER_LED_H
