#ifndef REFERENCIAMENTO_H
#define REFERENCIAMENTO_H

#include "mbed.h"

// Protótipo da função de homing
typedef void (*ReferenciarFunc)(void);
void referenciar(void);

// Variáveis de posição (passos acumulados)
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

// Sensores de fim de curso (definidos em main.cpp)
extern DigitalIn xMin;
extern DigitalIn xMax;
extern DigitalIn yMin;
extern DigitalIn yMax;
extern DigitalIn zMin;
extern DigitalIn zMax;

// Drivers de passo para X e Y (definidos em main.cpp)
extern DigitalOut DIR_X;
extern DigitalOut CLK_X;
extern DigitalOut ENABLE_X;
extern DigitalOut DIR_Y;
extern DigitalOut CLK_Y;
extern DigitalOut ENABLE_Y;

// Motor Z via BusOut (definido em main.cpp)
extern BusOut MP3;

#endif // REFERENCIAMENTO_H
