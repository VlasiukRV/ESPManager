
#include <Arduino.h>

#ifndef ESPMANAGER_LED_H
#define ESPMANAGER_LED_H

#define LED_MESSAGE_ON "on"
#define LED_MESSAGE_OFF "off"

class Led {

public:
    Led();
    Led(byte pin);
    Led(byte pin, char *name, char *synonym);

    byte getPin();
    const char* getName();
    const char* getSynonym();

    const char* getMqttTopic();
    void setMqttTopic(const char *mqtt_topic);

    void sendMessage(const char *message);

    void init(byte pin, const char *name, const char *synonym);
    void init();

    char* getState();
    void setState(char* state);
    void on();
    void off();

private:

    byte _pin;
    const char *_name;
    const char *_synonym;
    const char *_mqtt_topic;

    char *_state;
};

#endif //ESPMANAGER_LED_H
