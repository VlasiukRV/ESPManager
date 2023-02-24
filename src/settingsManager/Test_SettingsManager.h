//
// Created by vlasi on 2/23/2023.
//
#include "SettingsManager.h"

#ifndef ESPMANAGER_TEST_SETTINGSMANAGER_H
#define ESPMANAGER_TEST_SETTINGSMANAGER_H


class Test_SettingsManager {
public:

    static bool runTest() {
        Serial.println("BEGIN");

        Serial.println(" ----- TEST ESPProperty");
        ESPProperty _propertiesContainer[1];
        ESPProperty property1 = _propertiesContainer[0];
        property1.setName("Prop1");
        property1.setValue("Prop1Value");
        //printSettings(&_propertiesContainer[0]);

        const char * value = property1.getValueStr();
        const char * prop_name = property1.getName();
        //Serial.println(value);
        //Serial.println(prop_name);

        Serial.println(" ----- TEST Setting Manager 1");
        SettingsManager deviceSettings;
        Serial.println(" - addProperties deviceSettings1");
        deviceSettings.addSetting("ssid", "VRnet");
        deviceSettings.addSetting("password", "a m m e n !");
        deviceSettings.addSetting("delay", 1000);

        Serial.println(" - getSettings:");
        Serial.println(deviceSettings.getSettingStr("ssid"));
        Serial.println(deviceSettings.getSettingStr("password"));
        Serial.println(deviceSettings.getSettingInt("delay"));

        Serial.println("-----TEST Setting Manager 2");
        delay(1000);
        deviceSettings.processingSettings(printSettings);

        Serial.println("-----TEST Setting Manager 3");
        if (deviceSettings.saveSettings()) {
            Serial.println("-devise settings saved");
            delay(2000);
            SettingsManager deviceSettings2;
            Serial.println(" - addProperties deviceSettings2");
            deviceSettings2.addSetting("ssid", "");
            deviceSettings2.addSetting("password", "");
            deviceSettings2.addSetting("delay", 0);
            Serial.println(" - load settings");
            if (deviceSettings2.loadSettings()) {
                Serial.println(" - settings loaded:");
                deviceSettings2.processingSettings(printSettings);
            }

        }

        Serial.println("-----TEST Setting Manager 4");
        deviceSettings.processingSettings(printESPSettingsAsHTML);
        Serial.println("END");

        return true;
    }

    static void printESPSettingsAsHTML(ESPProperty *setting) {
        char s[512];
        const char table_row_text_html [] PROGMEM = R"rawliteral(<tr> <th> %s: </th>
                           <th> <form action="/post" method="post" target="hidden-form">
                            <input type="text" name="input_%s" value="%s">
                            <input type="submit" value="Submit" onclick="submitMessage()">
                            </form> </th> </tr>
                            )rawliteral";
        const char table_row_number_html [] PROGMEM = R"rawliteral(<tr> <th> %s: </th>
                    <th> <form action="/post" method="post" target="hidden-form">
                        <input type="number" name="input_%s" value="%i">
                                <input type="submit" value="Submit" onclick="submitMessage()">
                                        </form> </th> </tr>
                                        )rawliteral";

        switch (setting->getType()) {
            case CHAR:
                snprintf_P(s, sizeof(s), PSTR(table_row_text_html), setting->getName(), setting->getName(), setting->getValueStr());
                break;
            case INT:
                snprintf_P(s, sizeof(s), PSTR(table_row_number_html), setting->getName(), setting->getName(), setting->getValueInt());
                break;
            case LONG:
                snprintf_P(s, sizeof(s), PSTR(table_row_number_html), setting->getName(), setting->getName(), setting->getValueLong());
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
