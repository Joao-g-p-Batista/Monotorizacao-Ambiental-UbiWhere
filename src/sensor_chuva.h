#ifndef SENSOR_CHUVA_H
#define SENSOR_CHUVA_H

#include <Arduino.h>
#include <iostream>
#include <string>

float ler_sensor_chuva(int pin, int tempo_amostragem, float mm_por_pulso); 
// faz amostragem do sensor de chuva no pino 'pin' por 'tempo_amostragem' segundos
// retorna a média das leituras em mm/min
// pin: pino digital conectado ao sensor de chuva
// tempo_amostragem: tempo em segundos para fazer a amostragem
#endif

