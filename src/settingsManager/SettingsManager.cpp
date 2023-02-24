#include <Preferences.h>
#include "SettingsManager.h"

SettingsManager::SettingsManager() {

    _storageSpaceName = ESP32_STORAGE_SETTINGS_SPACE_NAME;

    _settingsLengthMax = PROPERTIES_CONTAINER_LENGTH;
    _settingsCounter = 0;

}

SettingsManager::~SettingsManager() {

    //delete []_propertiesContainer;

}

void SettingsManager::addSetting(char *name, int value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void SettingsManager::addSetting(char *name, long value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

void SettingsManager::addSetting(char *name, char *value) {

    if (_settingsCounter < _settingsLengthMax) {
        _propertiesContainer[_settingsCounter].setName(name);
        _propertiesContainer[_settingsCounter].setValue(value);
        _settingsCounter++;
    }

}

bool SettingsManager::loadSettings() {
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

bool SettingsManager::saveSettings() {
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

char *SettingsManager::getSettingStr(char *name) {
    ESPProperty property = _getSettingByName(name);
    return property.getValueStr();
}

int SettingsManager::getSettingInt(char *name) {
    ESPProperty property = _getSettingByName(name);
    return property.getValueInt();
}

long SettingsManager::getSettingLong(char *name) {
    ESPProperty property = _getSettingByName(name);
    return property.getValueLong();
}

ESPProperty SettingsManager::_getSettingByName(char *name) {

    for (int i = 0; i < _settingsCounter; i++) {

        if (strcmp(_propertiesContainer[i].getName(), name) == 0) {
            Serial.println(_propertiesContainer[i].getName());
            return _propertiesContainer[i];
        }

    }

    ESPProperty prop;
    return prop;

}

void SettingsManager::processingSettings(ProcesOnSetting Procedure) {

    for (int i = 0; i < _settingsCounter; i++) {
        Procedure(&this->_propertiesContainer[i]);
    }

}
