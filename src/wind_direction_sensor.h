#ifndef WIND_DIRECTION_SENSOR_H
#define WIND_DIRECTION_SENSOR_H

#include "weather_core.h"

#ifdef __cplusplus
extern "C" {
#endif

float WindDirectionSensor_getDirection(WeatherCore *core);

#ifdef __cplusplus
}
#endif

#endif
