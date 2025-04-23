#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"  // para acesso a x_posicao; y_posicao;

extern BusOut MP1;
extern BusOut MP2;
extern DigitalIn xMin, xMax, yMin, yMax;
extern int x_posicao, y_posicao;

#define velo 0.01f
#define MM_POR_STEP 4.0f    // passo do


static const uint8_t STEP_PATTERN[4] = {1<<0,1<<1,1<<2,1<<3};

static void step_forward(BusOut& M, int& pos) {
    for (int i = 0; i < 4; i++) {
        M = STEP_PATTERN[i];
        wait(velo);
    }
     pos += MM_POR_STEP;   // soma 4 mm
}

static void step_backward(BusOut& M, int& pos) {
    for (int i = 3; i >= 0; i--) {
        M = STEP_PATTERN[i];
        wait(velo);
    }
     pos -= MM_POR_STEP;   // soma 4 mm
}

void movimento_joystick(int x_joystick, int y_joystick) {
    // Eixo X: só avança se não atingiu o max
    if (x_joystick > 550 && xMax.read() == 1) {
        step_forward(MP1, x_posicao);
    }
    // Eixo X: só recua se não atingiu o min
    else if (x_joystick < 450 && xMin.read() == 1) {
        step_backward(MP1, x_posicao);
    }

    // Eixo Y: só avança se não atingiu o max
    if (y_joystick > 550 && yMax.read() == 1) {
        step_forward(MP2, y_posicao);
    }
    // Eixo Y: só recua se não atingiu o min
    else if (y_joystick < 450 && yMin.read() == 1) {
        step_backward(MP2, y_posicao);
    }
}
