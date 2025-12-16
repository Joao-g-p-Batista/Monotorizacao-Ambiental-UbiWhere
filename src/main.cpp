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
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

using namespace std;

// bibliotecas por sensor
#include "Classes.h"
#include "mqtt.h"
#include "weather_core.h"
#include "wind_speed_sensor.h"
#include "wind_direction_sensor.h"
#include "rain_sensor.h"
#include "light_sensor.h"
#include "sensor_temperatura_humidade.h"

// Sensores e variaveis globais
WeatherCore core;

sistema No_sistema;
sensor chuva;
sensor dir_vento;
sensor vel_vento;
sensor luz;
sensor temperatura;
sensor humidade;

// Definição dos pinos
// Na Olimex ESP32-POE, o pino 3 do UEXT é o GPIO 4 [cite: 395]
#define DHTPIN 4     
// Definindo o tipo de sensor
#define DHTTYPE DHT22
// Inicializa o sensor
DHT dht(DHTPIN, DHTTYPE);


// configuração do sensor de luminosidade
  LightSensorConfig_t lightConfig = {
    .sensorPin = 34,
    .samples = 20,
    .minRaw = 0,
    .maxRaw = 0
};
static LightSensor_t lightSensor;
static LightSensorData_t lightData;

// lista ponteiros sensores
sensor* lista_sensores[] = { &chuva, &dir_vento, &vel_vento };
int num_sensores = sizeof(lista_sensores) / sizeof(lista_sensores[0]);

// Wi-Fi credentials
const char* ssid = "iPhone";
const char* password = "atum12345";

// MQTT broker details
const char* mqtt_broker = "1c8370c6b65e49b48b8b05efc8f0f0db.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "Rede_sensores_IOT";
const char* mqtt_password = "!Q2w3e4r";

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
  // teste de configuração do nó MQTT
  //No_sistema = Configuracao_no();// faz configuração do nó MQTT ( local, cidade, id_nó )
  //função não funciona : (( erro de conversões de strings !!!!)
  
  // setup genérico
  Serial.begin(115200);
  No_sistema.cidade = "Aveiro";
  No_sistema.local = "Ubiwhere";
  No_sistema.id_no = "Nó_001";
 
  //inicializa o wi-fi e mqtt ( trocar por ethernet )
  setup_wifi();// conecta à rede Wi-Fi
  espClient.setInsecure(); // Usar conexão insegura para MQTT sobre TLS
  client.setServer(mqtt_broker, mqtt_port);// configura broker MQTT

  // Configuração dos sensores
  //chuva

  chuva.pino = 27; // pino do sensor de chuva
  chuva.amostragem_segundos = 10; // tempo de amostragem em segundos
  chuva.tipo_sensor = "sensor_chuva";
  chuva.unidade_medida = "MLM"; // milimetros minuto( not unitcode)
  chuva.tipo_leitura = "digital"; // leitura de pulsos
  // nota: 0.2794 mm por pulso

  // direção do vento
  
  dir_vento.pino = 32; // pino do sensor de direção do vento
  dir_vento.amostragem_segundos = 1; // tempo de amostragem em segundos
  dir_vento.tipo_sensor = "sensor_dir_vento";
  dir_vento.unidade_medida = "DEG"; // Degrees ( não está no unitcode)
  dir_vento.tipo_leitura = "analógico"; // divisor resistivo simples
  

  // velocidade do vento
  vel_vento.pino = 14; // pino do sensor de velocidade do vento
  vel_vento.amostragem_segundos = 2; // tempo de amostragem em segundos
  vel_vento.tipo_sensor = "sensor_vel_vento"; 
  vel_vento.unidade_medida = "KMH"; // km/h ( nâo está no unitcode)
  vel_vento.tipo_leitura = "digital"; // leitura de pulsos
  WindSpeedSensor windSpeed = { .windSpeedPin = vel_vento.pino };

  //luminosidade
  luz.pino = 34; // pino do sensor de luminosidade
  luz.amostragem_segundos = 1; // tempo de amostragem em segundos
  luz.tipo_sensor = "sensor_luz";
  luz.unidade_medida = "LUX"; // lux( está no unitcode )
  luz.tipo_leitura = "analógico"; // divisor resistivo simples

  // temperatura
  temperatura.valor = 0;
  temperatura.pino = DHTPIN; // pino do sensor de luminosidade
  temperatura.amostragem_segundos = 2; // tempo de amostragem em segundos
  temperatura.tipo_sensor = "sensor_temp"; // erro aqui, náo aceita o nome inteiro.( problema de memória ? )
  temperatura.unidade_medida = "TESTES"; // celsius( está no unitcode )
  temperatura.tipo_leitura = "digital"; // divisor resistivo simples


  //humidade
  humidade.pino = DHTPIN; // pino do sensor de luminosidade
  humidade.amostragem_segundos = 1; // tempo de amostragem em segundos
  humidade.tipo_sensor = "sensor_humidade";
  humidade.unidade_medida = "P1"; // lux( está no unitcode )
  humidade.tipo_leitura = "digital"; // divisor resistivo simples

  // setup weather kit 
  WeatherCore_init(&core, dir_vento.pino , vel_vento.pino, chuva.pino);
  lightSensor_init(&lightSensor, &lightConfig);
  WindSpeedSensor_init(&windSpeed);

  // inicia o sensor DHT
  dht.begin();

}

void loop() {

  string testes = " ";
  string mensagem = " ";
  

  //Le os sensores
  client.loop(); // 
  // leitura de sensores:
  dir_vento.valor = WindDirectionSensor_getDirection(&core); // graus
  vel_vento.valor = WindSpeedSensor_getSpeed(); // km/h
  chuva.valor = RainSensor_getTotalRain(&core); // mm/min

  lightSensor_readData(&lightSensor , &lightData ); // lux
  luz.valor = lightData.percentage; // percentagem de luminosidade

  temperatura.valor = ler_temperatura(dht); // graus Celsius
  humidade.valor = ler_humidade(dht); // percentagem de humidade


  //-----------------------------------------------
  Serial.print("Direction: "); Serial.println(dir_vento.valor);
  Serial.print("Speed: "); Serial.println(vel_vento.valor);
  Serial.print("Rain: "); Serial.println(chuva.valor);
  Serial.print("Light Percentage: "); Serial.println(luz.valor);
  Serial.print("Temperature: "); Serial.println(temperatura.valor);
  Serial.print("Humidity: "); Serial.println(humidade.valor);

  // Verifica a ligação MQTT

  if (!client.connected()) {
    reconnect();
  }
  // publica mensagem MQTT chuva



  testes = formatar_sistema(No_sistema, chuva);
  mensagem = formatar_sensor_ngsi_ld(chuva, No_sistema);
  Serial.println(testes.c_str());
  Serial.println(mensagem.c_str());
  client.publish(testes.c_str(), mensagem.c_str());

  // publica mensagem MQTT direção vento
  testes = formatar_sistema(No_sistema, dir_vento);
  mensagem = formatar_sensor_ngsi_ld(dir_vento, No_sistema);
  Serial.println(testes.c_str());
  Serial.println(mensagem.c_str());
  client.publish(testes.c_str(), mensagem.c_str());

  // publica mensagem MQTT velocidade vento
  testes = formatar_sistema(No_sistema, vel_vento);
  mensagem = formatar_sensor_ngsi_ld(vel_vento, No_sistema);
  Serial.println(testes.c_str());
  Serial.println(mensagem.c_str());
  client.publish(testes.c_str(), mensagem.c_str());


  // publica mensagem MQTT Luz
  testes = formatar_sistema(No_sistema, luz);
  mensagem = formatar_sensor_ngsi_ld(luz, No_sistema);
  Serial.println(testes.c_str());
  Serial.println(mensagem.c_str());
  client.publish(testes.c_str(), mensagem.c_str());

  // publica mensagem MQTT temperatura
  testes = formatar_sistema(No_sistema, temperatura);
  mensagem = formatar_sensor_ngsi_ld(temperatura, No_sistema);
  Serial.println(testes.c_str());
  Serial.println(mensagem.c_str());
  client.publish(testes.c_str(), mensagem.c_str());

  // publica mensagem MQTT humidade
  testes = formatar_sistema(No_sistema, humidade);
  mensagem = formatar_sensor_ngsi_ld(humidade, No_sistema);
  Serial.println(testes.c_str());
  Serial.println(mensagem.c_str());
  client.publish(testes.c_str(), mensagem.c_str());


  delay(10000); // delay 5 segundo
}
