#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <Arduino.h>

// Enumeração para os níveis de luminosidade
typedef enum {
    LIGHT_LEVEL_DARK,       // Escuro (0-10%)
    LIGHT_LEVEL_DIM,        // Pouca luz (10-30%)
    LIGHT_LEVEL_SHADOW,     // Sombra (30-60%)
    LIGHT_LEVEL_BRIGHT,     // Luminoso (60-85%)
    LIGHT_LEVEL_VERY_BRIGHT // Muito luminoso (85-100%)
} LightLevel_t;

// Estrutura para configuração do sensor de luminosidade
typedef struct {
    int sensorPin;
    int samples;
    uint16_t minRaw;        // Valor ADC mínimo (escuro total)
    uint16_t maxRaw;        // Valor ADC máximo (luz máxima)
} LightSensorConfig_t;

// Estrutura para os dados lidos do sensor
typedef struct {
    uint16_t raw;
    float voltage;
    float percentage;
    LightLevel_t level;
    const char* levelName;
} LightSensorData_t;

// Estrutura principal do sensor de luminosidade
typedef struct {
    LightSensorConfig_t config;
} LightSensor_t;

// Funções de interface do módulo
void lightSensor_init(LightSensor_t* sensor, LightSensorConfig_t* config);
void lightSensor_readData(LightSensor_t* sensor, LightSensorData_t* data);
void lightSensor_printData(LightSensorData_t* data);
void lightSensor_calibrate(LightSensor_t* sensor);

#endif // LIGHT_SENSOR_H
