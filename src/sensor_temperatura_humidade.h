#ifndef SENSOR_TEMPERATURA_HUMIDADE_H
#define SENSOR_TEMPERATURA_HUMIDADE_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Declaração das funções que vamos usar
// Elas recebem o sensor "dht" e devolvem um número (float)
float ler_temperatura(DHT &sensor);
float ler_humidade(DHT &sensor);

#endif