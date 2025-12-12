#ifndef MQTT_H
#define MQTT_H

#include <string>
#include "Classes.h"

//funções para MQTT

sistema Configuracao_no();
//pestana
//configurar o nó MQTT
//input: null
//durante a operação: obter cidade; obter local; obter Id_nó
//output: ponteira para a classe sistema

string formatar_sensor(sensor Sensor_atual);// sousa
//formatar dados em string
//input: dados do sensor, caracter especial
//Durante operação: ler os dados e criar uma string 
//ouptut: string estilo " {"temperature": 22.20}, {"Unidades": ºC} "

string formatar_sistema(sistema No_sistema,sensor Sensor_atual );// sousa
//formatar dados em string
//input: dados do sensor, caracter especial
//Durante operação: ler os dados e criar uma string 
//ouptut: string estilo " {"temperature": 22.20} "

// Versão NGSI_LD:
/*
{ "@context": ["http://uri.etsi.org/ngsi-ld/v1/ngsi-ld-core-context.jsonld"],  "id": "urn:ngsi-ld:ID_001:Sensor_Temperature",  "type": temperature,  "value": 25.5,  "units": "Celsius",
}
  */

//void publicar_mqtt(sistema No_sistema, sensor Sensor_atual);//batista
// nota: não fucniona pois a função de cliente apenas...
// funciona dentro de main
//enviar por mqtt
//input: ponteiro sistema, ponteiro do sensor
// durante operação: chamar a função de formatação e enviar com client.publish();
//Output " enviar a mensagem por mqtt "

#endif

