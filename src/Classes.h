// cabeçalho de ficheiro generico
// autor: João Batista
// data: 10/11/2025
// descrição: definição da classe sensor para uso geral em todas as funções de sensores

#ifndef CLASSES_H
#define CLASSES_H

#include <string>
using namespace std;

class sistema{
  public:
    string cidade="não definida";
    string local="não definido";
    string id_no="não definido";
};


class sensor {
  public:
    int pino=9999; // pino do sensor
    int valor=9999; // valor do sensor
    int amostragem_segundos=2; // tempo de amostragem em segundos
    string tipo_sensor=" não_definido "; // tipo de sensor ( chuva, vento, etc )
    string unidade_medida="não definido"; // unidade de medida ( mm, m/s, etc )
    //string flags="vazio"; // flags adicionais ( Ok, aviso, etc )
    string tipo_leitura = " não definido "; // analógico, digital, I2C, etc
};
#endif
