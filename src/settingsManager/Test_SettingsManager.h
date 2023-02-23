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
        property1.setName("ssid");
        property1.setValue("VRnet");

        Serial.println(_propertiesContainer[0].getValueStr());
        Serial.println(_propertiesContainer[0].getName());

        Serial.println(" ----- TEST Setting Manager 1");
        SettingsManager deviceSettings;
        Serial.println(" - addProperty:");
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

        Serial.println("-----TEST Setting Manager 2");
        if ( deviceSettings.saveSettings() ) {

            delay(2000);
            SettingsManager deviceSettings2;
            deviceSettings2.addSetting("ssid", "");
            deviceSettings2.addSetting("password", "");
            deviceSettings2.addSetting("delay", 0);

            if (deviceSettings2.loadSettings()) {
                deviceSettings2.processingSettings(printSettings);
            }

        }
        Serial.println("END");

        return true;
    }

    static void printSettings(ESPProperty* setting) {
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
