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

    ESPProperty(const char *name, property_types type);

    ESPProperty(const char *name, const char *synonym, property_types type);

    void setName(const char *name);

    const char *getName();

    void setSynonym(const char *synonym);

    const char *getSynonym();

    property_types getType();

    char *getValueStr();

    int getValueInt();

    long getValueLong();

    void setValue(char *current_value);

    void setValue(int current_value);

    void setValue(long current_value);

private:

    const char *_name;
    const char *_synonym;
    property_types _type;

    char *_char_value;
    int _int_value;
    long _long_value;

protected:

};

#endif //ESP32_ESPPROPERTY_H
