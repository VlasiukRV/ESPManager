#include <Preferences.h>
#include "ESPProperties.h"

ESPProperties::ESPProperties() {

    _storageSpaceName = ESP32_STORAGE_SETTINGS_SPACE_NAME;

    _settingsLengthMax = PROPERTIES_CONTAINER_LENGTH;
    _settingsCounter = 0;

}

ESPProperties::~ESPProperties() {

    //delete []_propertiesContainer;

}

void ESPProperties::addSetting(const char *name, int value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void ESPProperties::addSetting(const char *name, const char *synonym, int value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setSynonym(synonym);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void ESPProperties::addSetting(const char *name, long value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void ESPProperties::addSetting(const char *name, const char *synonym, long value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setSynonym(synonym);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void ESPProperties::addSetting(const char *name, char *value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void ESPProperties::addSetting(const char *name, const char *synonym, char *value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setSynonym(synonym);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

bool ESPProperties::loadSettings() {
    Preferences preferences;

    preferences.begin(_storageSpaceName, true);

    for (int i = 0; i < _settingsCounter; i++) {

        switch (_propertiesContainer[i].getType()) {
            case CHAR: {
                String str = preferences.getString(_propertiesContainer[i].getName(), "");
                const char *charPtr = str.c_str();
                _propertiesContainer[i].setValue(strdup(charPtr));
                break;
            }
            case INT:
                _propertiesContainer[i].setValue(preferences.getInt(_propertiesContainer[i].getName(), 0));
                break;
            case LONG:
                _propertiesContainer[i].setValue(preferences.getLong(_propertiesContainer[i].getName(), 0));
                break;
            default:
                break;
        }

    }

    preferences.end();

    return true;
}

bool ESPProperties::saveSettings() {
    Preferences preferences;

    preferences.begin(_storageSpaceName, false);

    for (int i = 0; i < _settingsCounter; i++) {

        switch (_propertiesContainer[i].getType()) {
            case CHAR:
                preferences.putString(_propertiesContainer[i].getName(), _propertiesContainer[i].getValueStr());
                break;
            case INT:
                preferences.putInt(_propertiesContainer[i].getName(), _propertiesContainer[i].getValueInt());
                break;
            case LONG:
                preferences.putLong(_propertiesContainer[i].getName(), _propertiesContainer[i].getValueLong());
                break;
            default:
                break;
        }

    }

    preferences.end();

    return true;

}

char *ESPProperties::getSettingStr(const char *name) {
    ESPProperty property = _getSettingByName(name);
    return property.getValueStr();
}

int ESPProperties::getSettingInt(const char *name) {
    ESPProperty property = _getSettingByName(name);
    return property.getValueInt();
}

long ESPProperties::getSettingLong(const char *name) {
    ESPProperty property = _getSettingByName(name);
    return property.getValueLong();
}

void ESPProperties::editSetting(const char *name, int value) {

    for (int i = 0; i < _settingsCounter; i++) {

        if (strcmp(_propertiesContainer[i].getName(), name) == 0) {
            _propertiesContainer[i].setValue(value);
        }

    }
}

void ESPProperties::editSetting(const char *name, long value) {
    for (int i = 0; i < _settingsCounter; i++) {

        if (strcmp(_propertiesContainer[i].getName(), name) == 0) {
            _propertiesContainer[i].setValue(value);
        }

    }
}

void ESPProperties::editSetting(const char *name, char *value) {
    for (int i = 0; i < _settingsCounter; i++) {

        if (strcmp(_propertiesContainer[i].getName(), name) == 0) {
            _propertiesContainer[i].setValue(value);
        }

    }
}

ESPProperty ESPProperties::_getSettingByName(const char *name) {

    for (int i = 0; i < _settingsCounter; i++) {

        if (strcmp(_propertiesContainer[i].getName(), name) == 0) {
            Serial.println(_propertiesContainer[i].getName());
            return _propertiesContainer[i];
        }

    }

    ESPProperty prop;
    return prop;

}

void ESPProperties::processingProperties(ProcesOnSetting Procedure) {

    for (int i = 0; i < _settingsCounter; i++) {
        Procedure(&this->_propertiesContainer[i]);
    }

}
