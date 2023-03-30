//
//
//

#include "ESPProperty.h"

#ifndef ESPPROPERTIES_H
#define ESPPROPERTIES_H

#define ESP32_STORAGE_SETTINGS_SPACE_NAME "settings_space"
#define PROPERTIES_CONTAINER_LENGTH 10

typedef std::function<void(ESPProperty *setting)> ProcesOnSetting;

class ESPProperties {
public:

    ESPProperties();

    ~ESPProperties();

    void addSetting(const char *name, int value);

    void addSetting(const char *name, const char *synonym, int value);

    void addSetting(const char *name, long value);

    void addSetting(const char *name, const char *synonym, long value);

    void addSetting(const char *name, char *value);

    void addSetting(const char *name, const char *synonym, char *value);

    void editSetting(const char *name, int value);
    void editSetting(const char *name, long value);
    void editSetting(const char *name, char *value);

    ESPProperty getSettingByName(const char *name);

    bool loadSettings();

    bool saveSettings();

    char *getSettingStr(const char *name);

    int getSettingInt(const char *name);

    long getSettingLong(const char *name);

    void processingProperties(ProcesOnSetting Procedure);

private:

    ESPProperty _propertiesContainer[PROPERTIES_CONTAINER_LENGTH];
    int _settingsLengthMax = PROPERTIES_CONTAINER_LENGTH;
    int _settingsCounter = 0;

    char *_storageSpaceName = ESP32_STORAGE_SETTINGS_SPACE_NAME;

protected:

};

#endif //ESP32_ESPPROPERTIES_H
