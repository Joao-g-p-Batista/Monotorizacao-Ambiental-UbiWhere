#include "sensor_temperatura_humidade.h"

// Função para ler APENAS a Temperatura
float ler_temperatura(DHT &sensor) {
    float t = sensor.readTemperature();
    
    // Se der erro (NaN), devolve um valor impossível (-999) para sabermos
    if (isnan(t)) {
        Serial.println(F("Falha ao ler temperatura!"));
        return -999.0;
    }
    return t;
}

// Função para ler APENAS a Humidade
float ler_humidade(DHT &sensor) {
    float h = sensor.readHumidity();
    
    if (isnan(h)) {
        Serial.println(F("Falha ao ler humidade!"));
        return -999.0;
    }
    return h;
}