#include "mbed.h"
#include "referenciamento.h"

// Motores (definidos em main.cpp)
extern BusOut MP1;           //  motor do eixo X
extern BusOut MP2;           //  motor do eixo Y
extern BusOut MP3;           //  motor do eixo Z

// Sensores de limite
DigitalIn xMin(D2), xMax(D13);   // sensores mínimos/máximos do X
DigitalIn yMin(D7), yMax(D12);   // sensores mínimos/máximos do Y 
DigitalIn zMin(D10), zMax(D11);  // sensores mínimos/máximos do Z

// Posições globais
int x_posicao = 0;           //  posição do X
int y_posicao = 0;           //  posição do Y
int z_posicao = 0;           //  posição do Z

#define VELO_HOMING    0.01f
#define BACKOFF_STEPS 10

static const uint8_t STEP_PATTERN[4] = {
    1<<0,  // fase 1
    1<<1,  // fase 2
    1<<2,  // fase 3
    1<<3   // fase 4
};

static void step_forward(BusOut& M, int& pos) {
    for (int i = 0; i < 4; i++) {
        M = STEP_PATTERN[i];
        wait(VELO_HOMING);
    }
    pos++;
}

static void step_backward(BusOut& M, int& pos) {
    for (int i = 3; i >= 0; i--) {
        M = STEP_PATTERN[i];
        wait(VELO_HOMING);
    }
    pos--;
}

void referenciar() {
    // Homing eixo X (mínimo)
    while (xMin.read() == 1) {
        step_backward(MP1, x_posicao);
    }
    // Back-off
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        step_forward(MP1, x_posicao);
    }
    x_posicao = 0;

    // Homing eixo Y (mínimo)
    while (yMin.read() == 1) {
        step_backward(MP2, y_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        step_forward(MP2, y_posicao);
    }
    y_posicao = 0;

    // Homing eixo Z (mínimo)
    while (zMin.read() == 1) {
        step_backward(MP3, z_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        step_forward(MP3, z_posicao);
    }
    z_posicao = 0;
}
