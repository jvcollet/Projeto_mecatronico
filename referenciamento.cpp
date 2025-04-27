// referenciamento.cpp
#include "mbed.h"
#include "referenciamento.h"

// Seleciona as funções de passo exportadas
extern void step_x(int direction, int &pos);
extern void step_y(int direction, int &pos);
extern void step_z(int direction, int &pos);

// Sensores e variáveis em main.cpp
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;
extern int       x_posicao, y_posicao, z_posicao;

#define BACKOFF_STEPS 10

void referenciar() {
    // Homing X
    while (xMin.read() == 1) {
        step_x(-1, x_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; ++i) {
        step_x(+1, x_posicao);
    }
    x_posicao = 0;

    // Homing Y
    while (yMin.read() == 1) {
        step_y(-1, y_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; ++i) {
        step_y(+1, y_posicao);
    }
    y_posicao = 0;

    // Homing Z
    while (zMin.read() == 1) {
        step_z(-1, z_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; ++i) {
        step_z(+1, z_posicao);
    }
    z_posicao = 0;
}
