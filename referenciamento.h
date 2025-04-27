#ifndef REFERENCIAMENTO_H
#define REFERENCIAMENTO_H

#include "mbed.h"

// Protótipo da função de homing
void referenciar(void);

// Sensores de fim de curso (definidos em main.cpp)
extern DigitalIn xMin;
extern DigitalIn xMax;
extern DigitalIn yMin;
extern DigitalIn yMax;
extern DigitalIn zMin;
extern DigitalIn zMax;

// Contadores de passos acumulados (definidos em main.cpp)
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

// Funções de passo unificadas (definidas em movimento.cpp)
void step_x(int direction, int &pos);
void step_y(int direction, int &pos);
void step_z(int direction, int &pos);

#endif // REFERENCIAMENTO_H