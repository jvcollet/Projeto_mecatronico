#ifndef MOVIMENTO_H
#define MOVIMENTO_H

#include "mbed.h"
#include "nextion_interface.h"

// Drivers de passo para X e Y (definidos em main.cpp)
extern DigitalOut DIR_X;
extern DigitalOut CLK_X;
extern DigitalOut ENABLE_X;
extern DigitalOut DIR_Y;
extern DigitalOut CLK_Y;
extern DigitalOut ENABLE_Y;

// Motor Z via BusOut (definido em main.cpp)
extern BusOut MP3;

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

// Função para movimentar motores usando o joystick
void movimento_manual(int x_joystick, int y_joystick, bool manual);

// Funções de passo unificadas (definidas em movimento.cpp)
void step_x(int direction, int &pos);
void step_y(int direction, int &pos);
void step_z(int direction, int &pos);

#define VELO_HOMING    200   // meio período de pulso (us)
#define VELO_HOMING_Z    2000   // meio período de pulso (us)

#endif // MOVIMENTO_H
