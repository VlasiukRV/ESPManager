//
//
//

#include "ESPProperty.h"
#ifndef SettingsManager_H
#define SettingsManager_H

#define ESP32_STORAGE_SETTINGS_SPACE_NAME "settings_space"
#define PROPERTIES_CONTAINER_LENGTH 3

class SettingsManager {
public:
    
    typedef void (*ProcesOnSetting) (ESPProperty* setting);
    
    SettingsManager();
    ~SettingsManager();
    void addSetting(char* name, int value);
    void addSetting(char* name, long value);
    void addSetting(char* name, char* value);

    bool loadSettings();
    bool saveSettings();

    char* getSettingStr(char* name);
    int getSettingInt(char* name);
    long getSettingLong(char* name);

    void processingSettings(ProcesOnSetting Procedure);
private:

    ESPProperty _getSettingByName(char* name);
    ESPProperty _propertiesContainer[PROPERTIES_CONTAINER_LENGTH];
    int _settingsLengthMax;
    int _settingsCounter;

    char* _storageSpaceName;
protected:

};

#endif //ESP32_SETTINGSMANAGER_H
