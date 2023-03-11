
#ifndef ESPMANAGER_SENSORSMANAGER_H
#define ESPMANAGER_SENSORSMANAGER_H

#include "Led.h"
#include "OledDisplay.h"

#define LED_SENSORS_CONTAINER_LENGTH 3

typedef std::function<void(Led *ledSensor)> ProcessLedSensor;

class SensorsManager {

public:

    SensorsManager();
    ~SensorsManager();

    void addLedSensor(int pin, const char *name, const char *synonym);
    void processingLedSensors(ProcessLedSensor Procedure);

private:

    Led _ledSensorsContainer[LED_SENSORS_CONTAINER_LENGTH];
    int _ledSensorsLengthMax;
    int _ledSensorsCounter;

};


#endif //ESPMANAGER_SENSORSMANAGER_H
