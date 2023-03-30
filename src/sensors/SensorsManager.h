
#ifndef ESPMANAGER_SENSORSMANAGER_H
#define ESPMANAGER_SENSORSMANAGER_H

#include "Led.h"
#include "DHTSensor.h"
#include "MotionSensor.h"
#include "OledDisplay.h"

#define LED_SENSORS_CONTAINER_LENGTH 3
#define DHT_SENSORS_CONTAINER_LENGTH 3
#define MOTION_SENSORS_CONTAINER_LENGTH 3

typedef std::function<void(Led *ledSensor)> ProcessLedSensor;
typedef std::function<void(DHTSensor *dhtSensor)> ProcessDHTSensor;
typedef std::function<void(MotionSensor *motionSensor)> ProcessMotionSensor;

class SensorsManager {

public:

    SensorsManager();
    ~SensorsManager();

    void addLedSensor(int pin, const char *name, const char *synonym);
    void addLedSensor(int pin, const char *name, const char *synonym, const char *mqtt_topic);
    void processingLedSensors(ProcessLedSensor Procedure);

    void addDHTSensor(int pin, const char *name, const char *synonym);
    void addDHTSensor(int pin, const char *name, const char *synonym, const char *mqtt_topic);
    void processingDHTSensors(ProcessDHTSensor Procedure);

    void addMotionSensor(int pin, const char *name, const char *synonym);
    void addMotionSensor(int pin, const char *name, const char *synonym, const char *mqtt_topic);
    void processingMotionSensors(ProcessMotionSensor Procedure);

private:

    Led _ledSensorsContainer[LED_SENSORS_CONTAINER_LENGTH];
    int _ledSensorsLengthMax = LED_SENSORS_CONTAINER_LENGTH-1;
    int _ledSensorsCounter = 0;

    DHTSensor _dhtSensorsContainer[DHT_SENSORS_CONTAINER_LENGTH];
    int _dhtSensorsLengthMax = DHT_SENSORS_CONTAINER_LENGTH-1;
    int _dhtSensorsCounter = 0;

    MotionSensor _motionSensorsContainer[MOTION_SENSORS_CONTAINER_LENGTH];
    int _motionSensorsLengthMax = DHT_SENSORS_CONTAINER_LENGTH-1;
    int _motionSensorsCounter = 0;

};


#endif //ESPMANAGER_SENSORSMANAGER_H
