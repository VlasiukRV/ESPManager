
#include "SensorsManager.h"

SensorsManager::SensorsManager() {

    _ledSensorsCounter = 0;
}

SensorsManager::~SensorsManager() {

}

void SensorsManager::addLedSensor(int pin, const char *name, const char *synonym) {

    if (_ledSensorsCounter < _ledSensorsLengthMax) {
        _ledSensorsContainer[_ledSensorsCounter].init(pin, name, synonym);
        _ledSensorsCounter++;
    }

}

void SensorsManager::processingLedSensors(ProcessLedSensor Procedure) {

    for (int i = 0; i < _ledSensorsCounter; i++) {
        Procedure(&this->_ledSensorsContainer[i]);
    }

}