#include "light_sensor.h"

// Função auxiliar privada para leitura suavizada
static uint16_t readSmooth(int pin, int samples)
{
    uint32_t sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(pin);
        delay(2); // curto atraso para estabilidade
    }
    return (uint16_t)(sum / samples);
}

// Função auxiliar para determinar o nível de luminosidade
static void determineLightLevel(LightSensorData_t* data)
{
    if (data->percentage < 10.0) {
        data->level = LIGHT_LEVEL_DARK;
        data->levelName = "Dark";
    } else if (data->percentage < 30.0) {
        data->level = LIGHT_LEVEL_DIM;
        data->levelName = "Low Light";
    } else if (data->percentage < 60.0) {
        data->level = LIGHT_LEVEL_SHADOW;
        data->levelName = "Shadow";
    } else if (data->percentage < 85.0) {
        data->level = LIGHT_LEVEL_BRIGHT;
        data->levelName = "Bright";
    } else {
        data->level = LIGHT_LEVEL_VERY_BRIGHT;
        data->levelName = "Maximum Light";
    }
}

// Inicializa o sensor de luminosidade
void lightSensor_init(LightSensor_t* sensor, LightSensorConfig_t* config)
{
    // Copia a configuração
    sensor->config = *config;
    
    // Configure attenuation para poder ler até ~3.3V
    analogSetPinAttenuation(sensor->config.sensorPin, ADC_11db);
    
    // Define valores padrão se não foram configurados
    if (sensor->config.minRaw == 0 && sensor->config.maxRaw == 0) {
        // Valores típicos para GL5528 com resistor de 10K
        // Escuro: ~50-200 ADC (resistência alta, pouca corrente)
        // Luz máxima: ~3000-4095 ADC (resistência baixa, muita corrente)
        sensor->config.minRaw = 100;   // Ajustar conforme necessário
        sensor->config.maxRaw = 4095;  // Máximo do ADC 12-bit
    }
    
    delay(100);
    
    Serial.println(F("=== Sensor de Luminosidade Inicializado ==="));
    Serial.printf("Range calibrado: %d - %d ADC\n", 
                  sensor->config.minRaw, sensor->config.maxRaw);
    Serial.println();
}

// Função de calibração (chamar em condições de luz mínima e máxima)
void lightSensor_calibrate(LightSensor_t* sensor)
{
    Serial.println(F("\n=== Calibração do Sensor de Luminosidade ==="));
    Serial.println(F("1. Cubra completamente o sensor (escuro total)"));
    Serial.println(F("   Aguarde 5 segundos..."));
    delay(5000);
    
    uint16_t minVal = readSmooth(sensor->config.sensorPin, 50);
    Serial.printf("Valor mínimo capturado: %d\n", minVal);
    
    Serial.println(F("\n2. Exponha o sensor à luz máxima disponível"));
    Serial.println(F("   Aguarde 5 segundos..."));
    delay(5000);
    
    uint16_t maxVal = readSmooth(sensor->config.sensorPin, 50);
    Serial.printf("Valor máximo capturado: %d\n", maxVal);
    
    // Atualiza a configuração
    sensor->config.minRaw = minVal;
    sensor->config.maxRaw = maxVal;
    
    Serial.println(F("\nCalibração concluída!"));
    Serial.printf("Novo range: %d - %d ADC\n\n", minVal, maxVal);
}

// Lê os dados do sensor de luminosidade
void lightSensor_readData(LightSensor_t* sensor, LightSensorData_t* data)
{
    // Média de leituras para estabilidade
    data->raw = readSmooth(sensor->config.sensorPin, sensor->config.samples);
    
    // ESP32 ADC 12-bit com atenuação 11db
    // Faixa aproximada: 0 ~ 2450mV (não linear)
    // Fórmula melhorada considerando a não-linearidade do ADC
    if (data->raw < 2048) {
        data->voltage = (data->raw / 2048.0) * 1.1;  // Zona mais linear
    } else {
        data->voltage = 1.1 + ((data->raw - 2048.0) / 2047.0) * 1.35;
    }
    
    // Converte para percentagem usando o range calibrado
    if (data->raw <= sensor->config.minRaw) {
        data->percentage = 0.0;
    } else if (data->raw >= sensor->config.maxRaw) {
        data->percentage = 100.0;
    } else {
        // Mapeamento linear entre min e max
        data->percentage = ((float)(data->raw - sensor->config.minRaw) / 
                           (float)(sensor->config.maxRaw - sensor->config.minRaw)) * 100.0;
    }
    
    // Limita entre 0 e 100%
    if (data->percentage < 0.0) data->percentage = 0.0;
    if (data->percentage > 100.0) data->percentage = 100.0;
    
    // Determina o nível de luminosidade
    determineLightLevel(data);
}

// Imprime os dados do sensor
void lightSensor_printData(LightSensorData_t* data)
{

    Serial.print("Light: ");
    Serial.print(data->percentage, 2);
    Serial.print("%  [");
    Serial.print(data->levelName);
    Serial.println("]");
}
