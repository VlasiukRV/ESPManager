#include <Arduino.h>

#ifndef ESPMANAGER_MOTIONSENSOR_H
#define ESPMANAGER_MOTIONSENSOR_H

class MotionSensor {
public:

    MotionSensor();
    ~MotionSensor();

    MotionSensor(byte pin);
    MotionSensor(byte pin, const char *name, const char *synonym);

    byte getPin();
    const char* getName();
    const char* getSynonym();

    const char* getMqttTopic();
    void setMqttTopic(const char *mqtt_topic);

    void init();
    void init(byte pin, const char *name, const char *synonym);

    bool getState();

    bool detectsMotion();

private:

    byte _pin;
    const char *_name;
    const char *_synonym;
    const char *_mqtt_topic;

    bool _state = false;

    int _pinStateCurrent    = LOW;  // current state of pin
    int _pinStatePrevious  = LOW;  // previous state of pin

};

#endif //ESPMANAGER_MOTIONSENSOR_H
