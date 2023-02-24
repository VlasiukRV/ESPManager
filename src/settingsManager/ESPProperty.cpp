//
// Created by vlasi on 2/20/2023.
//

#include "ESPProperty.h"

ESPProperty::ESPProperty() {

}

ESPProperty::ESPProperty(char *name, property_types type) {

    _int_value = 0;
    _long_value = 0;

    _name = name;
    _type = type;

}

void ESPProperty::setName(char *name) {
    _name = name;
}

char *ESPProperty::getName() {
    return _name;
}

property_types ESPProperty::getType() {
    return _type;
}

char *ESPProperty::getValueStr() {

    if (_type == CHAR) {
        return _char_value;
    } else {
        return 0;
    }

}

int ESPProperty::getValueInt() {

    if (_type == INT) {
        return _int_value;
    } else {
        return 0;
    }

}

long ESPProperty::getValueLong() {

    if (_type == LONG) {
        return _long_value;
    } else {
        return 0;
    }

}

void ESPProperty::setValue(char *current_value) {
    _type = CHAR;
    _char_value = current_value;
}

void ESPProperty::setValue(int current_value) {
    _type = INT;
    _int_value = current_value;
}

void ESPProperty::setValue(long current_value) {
    _type = LONG;
    _long_value = current_value;
}
