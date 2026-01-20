#include "wind_speed_sensor.h"
#include <Arduino.h>

void WindSpeedSensor_init(WindSpeedSensor *sensor)
{
    pinMode(sensor->windSpeedPin, INPUT_PULLUP);
}


float ler_sensor_vel_vento(int pin, int tempo_amostragem, float kmh_por_pulso) {
    //Serial.println("Lendo sensor de chuva...");        
    int contador_pulsos=0; // contador de Pulsos
    int timer=0; // em milisegundos
    int pulse_lock=0;
    int val= 1;

    while (timer<tempo_amostragem*1000){ // tempo_amostragem em milisegundos( segundos*1000)
        // lê o pino do sensor
        val = digitalRead(pin);
        if (val == 0 && pulse_lock == 0) {// verifica se houve mudança de estado do segundo anterior
            contador_pulsos = contador_pulsos + 1;
            pulse_lock = 1;  // bloqueia este pulso para náo repetir a contagem
        }    

        if (val == 1 && pulse_lock == 1)// desbloqueia o pulso quando o sinal volta ao estado normal
        {
            pulse_lock = 0;
        }

        // aguarda 1 milisegundo antes de nova leitura
        delay(5);
        timer = timer + 5;
    }
    return (float)((contador_pulsos/ tempo_amostragem )*2.4); // retorna 
}