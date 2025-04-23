#ifndef REFERENCIAMENTO_H
#define REFERENCIAMENTO_H

#include "mbed.h"

// Prototipo da função de homing
void referenciar();

// Variáveis de posição (passos desde a origem)
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

// Sensores de fim de curso (dependendo do pino, estarão em referenciamento.cpp)
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;

#endif // REFERENCIAMENTO_H
