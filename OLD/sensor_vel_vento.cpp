// sensor_velocidade_vento.cpp
// Generic wind speed sensor helper for ESP32/Arduino (PlatformIO)
// Place this file in src/ and include/use from your main sketch.
//
// Simple, dependency-light class that handles debouncing and optional callback.

#include <Arduino.h>
#include "sensor_vel_vento.h"

//nota: refazer a função de modo a poder devolver o valor e uma flag de confirmação.
// Ou, fazer uma função que verifica o valor e devolve uma flag de confirmação separadamente.
static volatile int contagemVoltas = 0;
static volatile unsigned long ultimaInterrupcao = 0;
static unsigned long ultimaVerificacao = 0;

void IRAM_ATTR contarClique() {
    unsigned long tempoAtual = millis();

    if (tempoAtual - ultimaInterrupcao > 10) {
        contagemVoltas++;
        ultimaInterrupcao = tempoAtual;
    }
}

float ler_sensor_velocidade_vento(int pin, int tempo_amostragem, float mm_por_pulso) {


static unsigned long inicioMedicao = 0;
    static bool primeiraExecucao = true;
    static bool a_medir = false;

    
    if (primeiraExecucao) {
        pinMode(pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin), contarClique, FALLING);
        contagemVoltas = 0;
        inicioMedicao = millis();
        primeiraExecucao = false;
        a_medir = true;
        return -1.0; 
    }

    // 2. Verificar se o tempo já passou
    unsigned long tempoAtual = millis();

    if (tempoAtual - inicioMedicao >= tempo_amostragem) {
        // O TEMPO ACABOU -> CALCULAR

        // Proteção de leitura atómica
        noInterrupts();
        int voltas = contagemVoltas;
        contagemVoltas = 0; // Reset para o próximo ciclo
        interrupts();

        // Atualizar o tempo de início para o próximo ciclo
        // Usamos a diferença real para precisão matemática
        float segundosReais = (tempoAtual - inicioMedicao) / 1000.0;
        inicioMedicao = tempoAtual; 

        // Cálculo da velocidade
        float velocidade = ((float)voltas / segundosReais) * mm_por_pulso;

        return velocidade; // <--- AQUI ESTÁ O VALOR FINAL
    }

    // 3. Se ainda não passou o tempo, retorna -1
    return -1.0;

}