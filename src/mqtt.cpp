#include "mqtt.h"
#include "Classes.h"
#include <string.h>
#include <Arduino.h>

sistema Configuracao_no(){
    sistema No_sistema;
    Serial.begin(115200);
    
        Serial.setTimeout(10000); // Timeout de 10 segundos

        // nota: a função não está a mostrar o input no monitor de série e não tem modo de confirmar se os dados estão corretos
        // estilo( estes são os resultados finais: estão corretos? (s/n)   ( S, s  = sim, termina e devolve os dados  N, n = não, volta ao inicio. ) )
        Serial.println("===== Configuração do Nó =====");
           
        // Ler Cidade
        Serial.println("Introduza a cidade: ");
        No_sistema.cidade = (Serial.readStringUntil('\n')).c_str() ;

        // Ler Local
        Serial.println("Introduza o local: ");
        No_sistema.local = (Serial.readStringUntil('\n')).c_str() ;

        // Ler Nó
        Serial.println("Introduza o identificador do nó: ");
        No_sistema.id_no = (Serial.readStringUntil('\n')).c_str() ;
        /*
        Serial.println("\n--- Configuração guardada ---");
        Serial.print("Cidade: "); 
        Serial.println(No_sistema.cidade.c_str());
        Serial.print("Local: "); 
        Serial.println(No_sistema.local.c_str());
        Serial.print("Nó: "); 
        Serial.println(No_sistema.id_no.c_str());
        Serial.println("==============================\n");
        */

        delay(4000); // espera 2 segundos antes de pedir confirmação

    return No_sistema;
} 

string formatar_sensor(sensor Sensor_atual){
    //exemplo: { "atum" : 25, "batata" : "frita" }
    string teste = ("{ \"" + Sensor_atual.tipo_sensor + "\" : " + to_string(Sensor_atual.valor) + ", \"unidades\" : \"" + Sensor_atual.unidade_medida + "\" }" );
    //Serial.println(teste.c_str());

    return teste;
}

string formatar_sistema(sistema No_sistema, sensor Sensor_atual){
      //exemplo: 
      // tópico: Aveiro/Ubiwhere/Nó_001/sensor_chuva
    string teste= (No_sistema.cidade + "/" + No_sistema.local + "/"+ No_sistema.id_no + "/" +Sensor_atual.tipo_sensor );
   // Serial.println(teste.c_str());

    return teste;
}

string formatar_sensor_ngsi_ld(sensor Sensor_atual, sistema No_sistema) {
    char valor_str[20];
    dtostrf(Sensor_atual.valor, 6, 2, valor_str);

    string mensagem = "{";
    mensagem += "\"@context\": [],";
    mensagem += "\"id\": \"urn:ngsi-ld:" + No_sistema.local + ":" + No_sistema.id_no + "\",";
    mensagem += "\"type\": \"EnvironmentalSensor\",";
    mensagem += "\"id_no\": {";
    mensagem += "\"type\": \"Property\",";
    mensagem += "\"value\": \"" + No_sistema.id_no + "\"";
    mensagem += "},";
    mensagem += "\"" + Sensor_atual.tipo_sensor + "\": {";
    mensagem += "\"type\": \"Property\",";
    mensagem += "\"value\": " + string(valor_str) + ",";
    mensagem += "\"unitCode\": \"" + Sensor_atual.unidade_medida + "\"";
    mensagem += "}";
    mensagem += "}";

    return mensagem;
}
