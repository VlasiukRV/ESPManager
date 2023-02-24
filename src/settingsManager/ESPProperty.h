//
// Created by vlasi on 2/20/2023.
//

#include <Arduino.h>

#ifndef ESP32_ESPPROPERTY_H
#define ESP32_ESPPROPERTY_H

enum property_types {
    STRING,
    CHAR,
    INT,
    LONG
};

enum property_html_types {
    TEXT,
    NUMBER
};

class ESPProperty {

public:

    ESPProperty();

    ESPProperty(char *name, property_types type);

    void setName(char *name);

    char *getName();

    property_types getType();

    char *getValueStr();

    int getValueInt();

    long getValueLong();

    void setValue(char *current_value);

    void setValue(int current_value);

    void setValue(long current_value);

private:

    char *_name;
    property_types _type;

    char *_char_value;
    int _int_value;
    long _long_value;

protected:

};

#endif //ESP32_ESPPROPERTY_H
