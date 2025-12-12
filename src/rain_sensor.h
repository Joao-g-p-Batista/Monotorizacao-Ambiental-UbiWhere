#ifndef RAIN_SENSOR_H
#define RAIN_SENSOR_H

#include "weather_core.h"

#ifdef __cplusplus
extern "C" {
#endif

float RainSensor_getTotalRain(WeatherCore *core);

#ifdef __cplusplus
}
#endif

#endif
