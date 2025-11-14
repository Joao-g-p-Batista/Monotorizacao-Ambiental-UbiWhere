// codigo principal do projeto
// estrutura basica do código:
// - inclui bibliotecas
// - define variaveis globais
// - configura conexao Wi-Fi e MQTT
// - teste de sensores
// - loop principal publica dados no broker MQTT
// - dormir entre leituras
// Versão 1: mensagem em char* ( texto corrido )

// bibliotecas gerais
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <iostream>
#include <string>
using namespace std;

// bibliotecas por sensor
#include "Classes.h"
#include "mqtt.h"
#include "sensor_chuva.h"
#include "sensor_dir_vento.h"
#include "sensor_vel_vento.h"

// Sensores e variaveis globais
sensor chuva;
sensor dir_vento;
sensor vel_vento;
//sensor sensor_temp;
//sensor sensor_co2;
//sensor sensor_co;
//sensor sensor_luminosidade;

// lista ponteiros sensores
sensor* lista_sensores[] = { &chuva, &dir_vento, &vel_vento };
int num_sensores = sizeof(lista_sensores) / sizeof(lista_sensores[0]);

// Wi-Fi credentials
const char* ssid = "Sony_jb";
const char* password = "carapau2000";

// MQTT broker details
const char* mqtt_broker = "1dc4c00d206c435ca94935a4c601f021.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "Rede_sensores_IOT";
const char* mqtt_password = "Senha1234";

// definições wifi
WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando à rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.print(chuva.valor);

  setup_wifi();
  espClient.setInsecure(); // Usar conexão insegura para MQTT sobre TLS
  client.setServer(mqtt_broker, mqtt_port);

  // Configuração dos sensores
  //chuva
  chuva.pino = 34; // pino do sensor de chuva
  chuva.amostragem_segundos = 10; // tempo de amostragem em segundos
  chuva.tipo_sensor = "sensor_chuva";
  chuva.unidade_medida = "mm/min";
  chuva.tipo_leitura = "digital"; // leitura de pulsos
  // nota: 0.2794 mm por pulso

  // direção do vento
  dir_vento.pino = 35; // pino do sensor de direção do vento
  dir_vento.amostragem_segundos = 10; // tempo de amostragem em segundos
  dir_vento.tipo_sensor = "sensor_dir_vento";
  dir_vento.unidade_medida = "graus"; // ver o datasheet para perceber os graus
  dir_vento.tipo_leitura = "analógico"; // divisor resistivo simples

  // velocidade do vento
  vel_vento.pino = 32; // pino do sensor de velocidade do vento
  vel_vento.amostragem_segundos = 10; // tempo de amostragem em segundos
  vel_vento.tipo_sensor = "sensor_vel_vento"; 
  vel_vento.unidade_medida = "km/h"; // nota: 2.4Km/h = 60 pulsos/min
  vel_vento.tipo_leitura = "digital"; // leitura de pulsos

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // 
  
  // leitura de sensores:
  chuva.valor = ler_sensor_chuva(chuva.pino, chuva.amostragem_segundos, 0.2794); // mm/min

  
  // publica mensagem MQTT

  // chama funçaõ publicar


  delay(20000); // delay 20 segundo
}