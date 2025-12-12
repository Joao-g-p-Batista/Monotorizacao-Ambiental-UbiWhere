#include "wind_direction_sensor.h"

float WindDirectionSensor_getDirection(WeatherCore *core)
{
    return core->kit->getWindDirection();
}
