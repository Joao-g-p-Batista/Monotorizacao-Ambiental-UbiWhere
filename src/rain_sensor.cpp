#include "rain_sensor.h"

float RainSensor_getTotalRain(WeatherCore *core)
{
    return core->kit->getTotalRainfall();
}
