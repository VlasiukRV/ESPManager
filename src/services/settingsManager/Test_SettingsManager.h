//
// Created by vlasi on 2/23/2023.
//
#include "ESPProperties.h"

#ifndef ESPMANAGER_TEST_SETTINGSMANAGER_H
#define ESPMANAGER_TEST_SETTINGSMANAGER_H


class Test_SettingsManager {
public:

    static bool testESPProperty() {

        Serial.println(F(" ----- TEST ESPProperty"));
        ESPProperty _propertiesContainer[1];
        ESPProperty *property1 = &_propertiesContainer[0];
        property1->setName("Prop1");
        property1->setValue("Prop1Value");
        printSettings(property1);

        const char *value = property1->getValueStr();
        const char *prop_name = property1->getName();
        Serial.println(value);
        Serial.println(prop_name);

        return true;
    }

    static bool testSettingsManager(ESPProperties *deviceSettings) {

        Serial.println(F(" ----- TEST Setting Manager 1"));

        Serial.println(F(" - addProperties deviceSettings1"));
        deviceSettings->addSetting("ssid", "WIFI SSID:", "VRnet");
        deviceSettings->addSetting("password", "WIFI PASSWORD:", "a m m e n !");
        deviceSettings->addSetting("delay", "INTERVAL for sending data to api:", 1000);

        Serial.println(F(" - getSettings:"));
        Serial.println(deviceSettings->getSettingStr("ssid"));
        Serial.println(deviceSettings->getSettingStr("password"));
        Serial.println(deviceSettings->getSettingInt("delay"));

        return true;

    }

    static bool testSavingSettingsManager(ESPProperties *deviceSettings) {

        Serial.println(F("-----TEST Setting Manager 2"));
        deviceSettings->processingProperties(printSettings);

        //if (deviceSettings->saveSettings()) {
        //Serial.println(F("-devise settings saved"));
        //} else {
        //return false;
        //}

    return true;
}

static bool testReadingSettingsManager(ESPProperties *deviceSettings) {
    Serial.println(F("-----TEST Setting Manager 3"));

    Serial.println(F(" - addProperties deviceSettings2"));
    deviceSettings->addSetting("ssid", "WIFI SSID:", "");
    deviceSettings->addSetting("password", "WIFI PASSWORD:", "");
    deviceSettings->addSetting("delay", "INTERVAL for sending data to api:", 0);
    Serial.println(F(" - load settings"));
    if (deviceSettings->loadSettings()) {
        Serial.println(F(" - settings loaded:"));
        deviceSettings->processingProperties(printSettings);
    } else {
        return false;
    }

    return true;
}

    static bool testProcessingSettingsManager(ESPProperties *deviceSettings, SettingsManager *deviceSettings1) {

        Serial.println(F("-----TEST Setting Manager 4"));
        deviceSettings->processingProperties(printESPSettingsAsHTML);
        deviceSettings->processingProperties(printSettings);
        deviceSettings1->processingProperties(printSettings);

        return true;

    }
static bool runTest() {
    Serial.println(F("BEGIN"));

    bool testESPPropertyResult = testESPProperty();

    ESPProperties deviceSettings;
    ESPProperties deviceSettings1;
    bool testSettingsManagerResult = testSettingsManager(&deviceSettings);
    bool testSavingSettingsManagerResult = testSavingSettingsManager(&deviceSettings);
    if (testSavingSettingsManagerResult) {
        bool testReadingSettingsManagerResult = testReadingSettingsManager(&deviceSettings1);
    }

    bool testProcessingSettingsManagerResult = testProcessingSettingsManager(&deviceSettings, &deviceSettings1);

    Serial.println(F("END"));

    return true;
}

static void printESPSettingsAsHTML(ESPProperty *setting) {
    char s[512];
    const char table_row_text_html[]
    PROGMEM = R"rawliteral(<tr> <th> %s: </th>
                           <th> <form action="/post" method="post" target="hidden-form">
                            <input type="text" name="input_%s" value="%s">
                            <input type="submit" value="Submit" onclick="submitMessage()">
                            </form> </th> </tr>
                            )rawliteral";
    const char table_row_number_html[]
    PROGMEM = R"rawliteral(<tr> <th> %s: </th>
                    <th> <form action="/post" method="post" target="hidden-form">
                        <input type="number" name="input_%s" value="%i">
                                <input type="submit" value="Submit" onclick="submitMessage()">
                                        </form> </th> </tr>
                                        )rawliteral";

    switch (setting->getType()) {
        case CHAR:
            snprintf_P(s, sizeof(s), PSTR(table_row_text_html), setting->getSynonym(), setting->getName(),
                       setting->getValueStr());
            break;
        case INT:
            snprintf_P(s, sizeof(s), PSTR(table_row_number_html), setting->getSynonym(), setting->getName(),
                       setting->getValueInt());
            break;
        case LONG:
            snprintf_P(s, sizeof(s), PSTR(table_row_number_html), setting->getSynonym(), setting->getName(),
                       setting->getValueLong());
            break;
    }
    Serial.println(s);

}

static void printSettings(ESPProperty *setting) {
    char s[32];
    switch (setting->getType()) {
        case CHAR:
            snprintf_P(s, sizeof(s), PSTR("Prop %s is %s "), setting->getName(), setting->getValueStr());
            break;
        case INT:
            snprintf_P(s, sizeof(s), PSTR("Prop %s is %i "), setting->getName(), setting->getValueInt());
            break;
        case LONG:
            snprintf_P(s, sizeof(s), PSTR("Prop %s is %i "), setting->getName(), setting->getValueLong());
            break;
    }
    Serial.println(s);
}

};


#endif //ESPMANAGER_TEST_SETTINGSMANAGER_H
