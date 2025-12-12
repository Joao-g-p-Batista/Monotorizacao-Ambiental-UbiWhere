#ifndef WEATHER_CORE_H
#define WEATHER_CORE_H

#include <Arduino.h>
#include "SparkFun_Weather_Meter_Kit_Arduino_Library.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    SFEWeatherMeterKit *kit;
} WeatherCore;

void WeatherCore_init(WeatherCore *core,
                      int windDirectionPin,
                      int windSpeedPin,
                      int rainfallPin);

#ifdef __cplusplus
}
#endif

#endif
