// cabeçalho de ficheiro generico
// autor: João Batista
// data: 10/11/2025
// descrição: definição da classe sensor para uso geral em todas as funções de sensores

#ifndef CLASSES_H
#define CLASSES_H

#include <string>
using namespace std;

// classe que guarda as informações do nó 
class sistema{  
  public:
    string cidade="não definida"; // localização do Nó
    string local="não definido";  // localização mais concreta do nó
    string id_no="não definido";  // Identificador do Nó ( sensor_Node_XXX )
};

// classe que guarda as informações de um dado sensor
// cada sensor que for colocado deve estar associado a uma variavel/classe "sensor"
// exemplo:
// sensor temperatura; ( sensor= variavel ; temperatura= nome)
class sensor {
  public:
    int pino=9999;                          // pino do sensor
    float valor=9999;                       // valor do sensor
    int amostragem_segundos=2;              // tempo de amostragem em segundos
    string tipo_sensor=" não_definido";    // tipo de sensor ( chuva, vento, etc )
    string unidade_medida="não definido";   // unidade de medida ( mm, m/s, etc ) em unitcode UN/Cefact codes
    int maximo_medida =  999999;            // valor maximo que o sensor devia ler
    int minimo_medida = -999999;            // Valor minimo que o sensor devia les
    string flags="Vazio";                   // flags de leitura ( Ok, NOk)
    string tipo_leitura = "não definido_kjsbfiubeifkn "; // analógico, digital, I2C, etc
};
#endif
