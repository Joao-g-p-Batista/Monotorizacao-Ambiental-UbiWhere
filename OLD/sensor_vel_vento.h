// sensor_vel_vento.h
// Generic header for a sensor de velocidade do vento
// Minimal, portable interface suitable for ESP32/Arduino projects.
#ifndef SENSOR_VEL_VENTO_H
#define SENSOR_VEL_VENTO_H

#include <Arduino.h>
#include <iostream>
#include <string>

float ler_sensor_velocidade_vento(int pin, int tempo_amostragem); 
// faz amostragem do sensor de velocidade de vento no pino 'pin' por 'tempo_amostragem' segundos
// retorna a média das leituras em km/h
// pin: pino digital conectado ao sensor de chuva
// tempo_amostragem: tempo em segundos para fazer a amostragem
#endif
