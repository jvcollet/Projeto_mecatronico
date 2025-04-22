#include "mbed.h"
#include "referenciamento.h"

// Motores (definidos em main.cpp)
extern BusOut MP1;
extern BusOut MP2;

// Sensores
DigitalIn xMin(D2), xMax(D13), yMin(D7), yMax(D12);

// Posições globais
int x_posicao = 0;
int y_posicao = 0;

#define VELO_HOMING    0.01f
#define BACKOFF_STEPS 10

static const uint8_t STEP_PATTERN[4] = {1<<0, 1<<1, 1<<2, 1<<3};

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
    // Pequeno back‑off
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
}
