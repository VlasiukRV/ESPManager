#include <Preferences.h>
#include "ESPProperties.h"

ESPProperties::ESPProperties() {



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
                Serial.printf( "    -- load string [%s] = [%s] \n", _propertiesContainer[i].getName(), charPtr );
                break;
            }
            case INT: {
                int intValue = preferences.getInt(_propertiesContainer[i].getName(), 0);
                _propertiesContainer[i].setValue(intValue);
                Serial.printf( "    -- load int [%s] = [%i] \n", _propertiesContainer[i].getName(), _propertiesContainer[i].getValueInt() );
                break;
            }
            case LONG: {
                long longValue;
                longValue = preferences.getLong(_propertiesContainer[i].getName(), 0);
                _propertiesContainer[i].setValue(longValue);
                Serial.printf( "    -- load long [%s] = [%i] \n", _propertiesContainer[i].getName(), _propertiesContainer[i].getValueLong() );
                break;
            }
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
            case CHAR: {
                Serial.printf( "    -- save string [%s] = [%s] \n", _propertiesContainer[i].getName(), _propertiesContainer[i].getValueStr() );
                preferences.putString(_propertiesContainer[i].getName(), _propertiesContainer[i].getValueStr());
                break;
                }
            case INT: {
                Serial.printf( "    -- save int [%s] = [%i] \n", _propertiesContainer[i].getName(), _propertiesContainer[i].getValueInt() );
                preferences.putInt(_propertiesContainer[i].getName(), _propertiesContainer[i].getValueInt());
                break;
                }
            case LONG: {
                Serial.printf( "    -- save long [%s] = [%i] \n", _propertiesContainer[i].getName(), _propertiesContainer[i].getValueLong() );
                preferences.putLong(_propertiesContainer[i].getName(), _propertiesContainer[i].getValueLong());
                break;
                }
            default:
                break;
        }

    }

    preferences.end();

    return true;

}

bool ESPProperties::loadESPConfiguration(DynamicJsonDocument doc) {

    //Preferences preferences;
    //preferences.begin(_storageConfigurationSpaceName, true);

    //String str = preferences.getString("configuration", "");
    Serial.println("Load configurationJSON:");
    //Serial.println(str);

    //const char *json = str.c_str();

    //DeserializationError error = deserializeJson(doc, json);

    File file = SPIFFS.open("/configuration.json", FILE_READ);
    if (file && file.size()) {
        Serial.printf("read file ");
        DeserializationError error = deserializeJson(doc, file);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        file.close();
        return true;
    }
    file.close();
    //preferences.end();

    return false;

}

bool ESPProperties::saveESPConfiguration(DynamicJsonDocument doc) {

    File file = SPIFFS.open("/configuration.json", FILE_WRITE);
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }
    file.close();

}
//bool ESPProperties::saveESPConfiguration(char *configurationJSON) {
//
//    Preferences preferences;
//    preferences.begin(_storageConfigurationSpaceName, false);
//
//    Serial.println("Save configurationJSON:");
//    Serial.println(configurationJSON);
//
//    preferences.putString("configuration", configurationJSON);
//
//    preferences.end();
//
//    return true;
//
//}

char *ESPProperties::getSettingStr(const char *name) {
    ESPProperty property = getSettingByName(name);
    return property.getValueStr();
}

int ESPProperties::getSettingInt(const char *name) {
    ESPProperty property = getSettingByName(name);
    return property.getValueInt();
}

long ESPProperties::getSettingLong(const char *name) {
    ESPProperty property = getSettingByName(name);
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

ESPProperty ESPProperties::getSettingByName(const char *name) {

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
