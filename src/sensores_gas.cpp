#include "Sensores_Gas.h"

// Definições das variáveis globais 
esp_adc_cal_characteristics_t adc_chars_0db;
esp_adc_cal_characteristics_t adc_chars_11db;
RangeMode currentRange = LOW_RANGE;

const int THRESH_HIGH_mV = 1050;   // sobe para 11dB acima disto
const int THRESH_LOW_mV  = 900;    // volta para 0dB abaixo disto

void initCalibration()
{
  // 0 dB
  esp_adc_cal_characterize(
    ADC_UNIT_1,
    ADC_ATTEN_DB_0,
    ADC_WIDTH_BIT_12,
    0,
    &adc_chars_0db
  );

  // 11 dB
  esp_adc_cal_characterize(
    ADC_UNIT_1,
    ADC_ATTEN_DB_11,
    ADC_WIDTH_BIT_12,
    0,
    &adc_chars_11db
  );
}

uint32_t readCalibratedmV(int pino)
{
  int raw = analogRead(pino);

  if (currentRange == LOW_RANGE)
    return esp_adc_cal_raw_to_voltage(raw, &adc_chars_0db);
  else
    return esp_adc_cal_raw_to_voltage(raw, &adc_chars_11db);
}

void ensureRange(uint32_t mv, int pino)
{
  if (currentRange == LOW_RANGE && mv > THRESH_HIGH_mV)
  {
    currentRange = HIGH_RANGE;
    analogSetPinAttenuation(pino, ADC_11db);
    delay(5);
  }
  else if (currentRange == HIGH_RANGE && mv < THRESH_LOW_mV)
  {
    currentRange = LOW_RANGE;
    analogSetPinAttenuation(pino, ADC_0db);
    delay(5);
  }
}

float readGasPPM_CO(int pino)
{
  // 1ª leitura
  uint32_t mv = readCalibratedmV(pino);

  // Ver se é preciso mudar de escala
  ensureRange(mv,pino);

  // Se mudou de escala, ler novamente já calibrado nessa escala
  mv = readCalibratedmV(pino);

  float v_adc = mv / 1000.0;            // Volts no pino do ESP32
  float v_sensor = v_adc * DIV_FACTOR;  // Volts reais no sensor

  float Rs = RL * (VC - v_sensor) / v_sensor;
  float ratio = Rs / R0;

  // MQ-7: calcular PPM
  float ppm = A * pow(ratio, B);

  return ppm;
}


void initCalibration_CO2()
{
  // 0 dB
  esp_adc_cal_characterize(
    ADC_UNIT_1,
    ADC_ATTEN_DB_0,
    ADC_WIDTH_BIT_12,
    0,
    &adc_chars_0db
  );

  // 11 dB
  esp_adc_cal_characterize(
    ADC_UNIT_1,
    ADC_ATTEN_DB_11,
    ADC_WIDTH_BIT_12,
    0,
    &adc_chars_11db
  );
}

uint32_t readCalibratedmV_CO2(int pino)
{
  int raw = analogRead(pino);

  if (currentRange == LOW_RANGE)
    return esp_adc_cal_raw_to_voltage(raw, &adc_chars_0db);
  else
    return esp_adc_cal_raw_to_voltage(raw, &adc_chars_11db);
}

void ensureRange_CO2(uint32_t mv, int pino)
{
  if (currentRange == LOW_RANGE && mv > THRESH_HIGH_mV)
  {
    currentRange = HIGH_RANGE;
    analogSetPinAttenuation(pino, ADC_11db);
    delay(5);
  }
  else if (currentRange == HIGH_RANGE && mv < THRESH_LOW_mV)
  {
    currentRange = LOW_RANGE;
    analogSetPinAttenuation(pino, ADC_0db);
    delay(5);
  }
}


float readGasPPM_CO2(int pino)
{
  // 1ª leitura
  uint32_t mv = readCalibratedmV_CO2(pino);

  // Ver se é preciso mudar de escala
  ensureRange_CO2(mv, pino);

  // Se mudou de escala, ler novamente já calibrado nessa escala
  mv = readCalibratedmV_CO2(pino);

  float v_adc = mv / 1000.0;            // Volts no pino do ESP32
  float v_sensor = v_adc * DIV_FACTOR;  // Volts reais no sensor

  float Rs = RL_CO2 * (VC - v_sensor) / v_sensor;
  float ratio = Rs / R0_CO2;

  // MQ-135
  float ppm = C * pow(ratio, D);

  return ppm;
}
