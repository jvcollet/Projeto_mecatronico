// referenciamento.cpp
#include "mbed.h"
#include "referenciamento.h"

// Drivers de passo definidos em main.cpp
extern DigitalOut DIR_X;
extern DigitalOut CLK_X;
extern DigitalOut ENABLE_X;
extern DigitalOut DIR_Y;
extern DigitalOut CLK_Y;
extern DigitalOut ENABLE_Y;

// Motor Z via BusOut (definido em main.cpp)
extern BusOut MP3;

// Sensores de limite (definidos em main.cpp)
extern DigitalIn xMin;
extern DigitalIn xMax;
extern DigitalIn yMin;
extern DigitalIn yMax;
extern DigitalIn zMin;
extern DigitalIn zMax;

// Contadores de passos acumulados (definidos em referenciamento.h)
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

#define VELO_HOMING    0.001f   // meio período de pulso (s)
#define BACKOFF_STEPS 10        // passos de back-off pós homing

// Padrão de fases para motor unipolar/stepper
static const uint8_t STEP_PATTERN[4] = {
    1<<0,  // fase 1
    1<<1,  // fase 2
    1<<2,  // fase 3
    1<<3   // fase 4
};

// Pulso único para drivers de passo (X e Y)
static void pulso_step(DigitalOut &CLK) {
    CLK = 1;
    wait(VELO_HOMING);
    CLK = 0;
    wait(VELO_HOMING);
}

// Eixo X
static void step_forward_x(int &pos) {
    DIR_X    = 1;
    ENABLE_X = 0;    // habilita driver (ativo baixo)
    pulso_step(CLK_X);
    pos++;
}
static void step_backward_x(int &pos) {
    DIR_X    = 0;
    ENABLE_X = 0;
    pulso_step(CLK_X);
    pos--;
}

// Eixo Y
static void step_forward_y(int &pos) {
    DIR_Y    = 1;
    ENABLE_Y = 0;
    pulso_step(CLK_Y);
    pos++;
}
static void step_backward_y(int &pos) {
    DIR_Y    = 0;
    ENABLE_Y = 0;
    pulso_step(CLK_Y);
    pos--;
}

// Eixo Z (BusOut, padrão antigo)
static void step_forward_z(int &pos) {
    for (int i = 0; i < 4; i++) {
        MP3 = STEP_PATTERN[i];
        wait(VELO_HOMING);
    }
    pos++;
}
static void step_backward_z(int &pos) {
    for (int i = 3; i >= 0; i--) {
        MP3 = STEP_PATTERN[i];
        wait(VELO_HOMING);
    }
    pos--;
}

void referenciar() {
    // Homing eixo X (mínimo)
    while (xMin.read() == 1) {
        step_backward_x(x_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        step_forward_x(x_posicao);
    }
    x_posicao = 0;

    // Homing eixo Y (mínimo)
    while (yMin.read() == 1) {
        step_backward_y(y_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        step_forward_y(y_posicao);
    }
    y_posicao = 0;

    // Homing eixo Z (mínimo)
    while (zMin.read() == 1) {
        step_backward_z(z_posicao);
    }
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        step_forward_z(z_posicao);
    }
    z_posicao = 0;
}
