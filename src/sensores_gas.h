#ifndef SENSORES_GAS_H
#define SENSORES_GAS_H

#include <Arduino.h>
#include "esp_adc_cal.h"

// ---------------- CONFIG ----------------
//#define MQ7_PIN 33
//#define MQ135_PIN 32

// Alimentação do sensor
#define VC 5.0

// Constantes da curva do MQ-7 
#define A 100.0
#define B -1.5

#define RL 10.0          // Resistência de carga em kΩ
#define R1 10.0          // Resistor divisor em kΩ
#define R2 18.0          // Resistor divisor em kΩ
#define R0 10.0          // Valor de R0 após calibração (substituir!)
#define DIV_FACTOR ((R1 + R2) / R2)

// MQ-135

// Constantes da curva do MQ-135
#define C 116.6020682
#define D -2.769034857

#define RL_CO2 10.0
#define R0_CO2 120.0

// Declarações (extern)
extern esp_adc_cal_characteristics_t adc_chars_0db;
extern esp_adc_cal_characteristics_t adc_chars_11db;

enum RangeMode { LOW_RANGE, HIGH_RANGE };
extern RangeMode currentRange;

// Histerese
extern const int THRESH_HIGH_mV;   // sobe para 11dB acima disto
extern const int THRESH_LOW_mV;    // volta para 0dB abaixo disto

void initCalibration();
uint32_t readCalibratedmV(int pino);
void ensureRange(uint32_t mv, int pino);
float readGasPPM_CO(int pino);

void initCalibration_CO2();
uint32_t readCalibratedmV_CO2(int pino);
void ensureRange_CO2(uint32_t mv, int pino);
float readGasPPM_CO2(int pino);

#endif
