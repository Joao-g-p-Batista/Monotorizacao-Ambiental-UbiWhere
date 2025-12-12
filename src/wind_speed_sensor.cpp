#include "wind_speed_sensor.h"
#include <Arduino.h>

// Variáveis da ISR
static volatile int contagemVoltas = 0;
static volatile unsigned long ultimaInterrupcao = 0;

void IRAM_ATTR contarClique()
{
    unsigned long tempoAtual = millis();
    if (tempoAtual - ultimaInterrupcao > 10) {
        contagemVoltas++;
        ultimaInterrupcao = tempoAtual;
    }
}

void WindSpeedSensor_init(WindSpeedSensor *sensor)
{
    pinMode(sensor->windSpeedPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(sensor->windSpeedPin), contarClique, FALLING);

    contagemVoltas = 0;
    ultimaInterrupcao = 0;
}

float WindSpeedSensor_getSpeed()
{
    noInterrupts();
    int voltas = contagemVoltas;
    contagemVoltas = 0;
    interrupts();

    return voltas * 2.4f;
}
