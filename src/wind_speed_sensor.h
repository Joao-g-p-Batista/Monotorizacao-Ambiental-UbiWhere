#ifndef WIND_SPEED_SENSOR_H
#define WIND_SPEED_SENSOR_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int windSpeedPin;
} WindSpeedSensor;

void WindSpeedSensor_init(WindSpeedSensor *sensor);
float WindSpeedSensor_getSpeed(); // SEM argumentos

#ifdef __cplusplus
}
#endif

#endif
