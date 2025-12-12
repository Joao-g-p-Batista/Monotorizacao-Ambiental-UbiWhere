// sensor_chuva.cpp
// Generic rain sensor helper for ESP32/Arduino (PlatformIO)
// Place this file in src/ and include/use from your main sketch.
//
// Simple, dependency-light class that handles debouncing and optional callback.

#include <Arduino.h>
#include "sensor_chuva.h"

//nota: refazer a função de modo a poder devolver o valor e uma flag de confirmação.
// Ou, fazer uma função que verifica o valor e devolve uma flag de confirmação separadamente.


float ler_sensor_chuva(int pin, int tempo_amostragem, float mm_por_pulso) {
    Serial.println("Lendo sensor de chuva...");        
    int contador_pulsos=0; // contador de Pulsos
    int timer=0; // em milisegundos
    int pulse_lock=0;
    int val= 1;


    while (timer<tempo_amostragem*1000){ // tempo_amostragem em milisegundos( minutos*60*1000)
        // lê o pino do sensor
        val = digitalRead(pin);

        Serial.print("valor lido do pino: ");
        Serial.print(val);  
        Serial.print("\n");

        if (val == 0 && pulse_lock == 0) {// verifica se houve mudança de estado do segundo anterior
            Serial.print("loop - pulso detectado");
            contador_pulsos = contador_pulsos + 1;
            pulse_lock = 1;  // bloqueia este pulso para náo repetir a contagem
        }
    
        if (val == 1 && pulse_lock == 1)// desbloqueia o pulso quando o sinal volta ao estado normal
        {
            pulse_lock = 0;
        }
        
        // aguarda 10 milisegundo antes de nova leitura
        delay(100);
        timer = timer + 100;
    }
        Serial.print("total de pulsos detectados: ");
        Serial.print(contador_pulsos);
        Serial.print("\n");
        
        Serial.print("chuva em mm/min: ");
        Serial.print( (contador_pulsos * mm_por_pulso) / tempo_amostragem ) ;   
        Serial.print("\n");

    return ( (contador_pulsos * mm_por_pulso) / tempo_amostragem ); // retorna mm/min

}