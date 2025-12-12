#include "weather_core.h"

void WeatherCore_init(WeatherCore *core,
                      int windDirectionPin,
                      int windSpeedPin,
                      int rainfallPin)
{
    pinMode(rainfallPin, INPUT_PULLUP);   // ← necessário

    core->kit = new SFEWeatherMeterKit(windDirectionPin,
                                       windSpeedPin,
                                       rainfallPin);

#ifdef SFE_WMK_PLAFTORM_UNKNOWN
    core->kit->setADCResolutionBits(10);
#endif

    core->kit->begin();  // ← interrupções do pluviômetro são ativadas aqui
}
