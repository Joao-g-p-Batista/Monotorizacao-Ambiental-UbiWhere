#ifndef MQTT_H
#define MQTT_H

#include <string>
#include "Classes.h"

//funções para MQTT

sistema Configuração_nó();
//pestana
//configurar o nó MQTT
//input: null
//durante a operação: obter cidade; obter local; obter Id_nó
//output: ponteira para a classe sistema

string formatar_dados(sensor Sensor_atual);// sousa
//formatar dados em string
//input: dados do sensor, caracter especial
//Durante operação: ler os dados e criar uma string 
//ouptut: string estilo " {"temperature": 22.20} "

void publicar_mqtt(sistema No_sistema, sensor Sensor_atual);//batista
//enviar por mqtt
//input: ponteiro sistema, ponteiro do sensor
// durante operação: chamar a função de formatação e enviar com client.publish();
//Output " enviar a mensagem por mqtt "

#endif

