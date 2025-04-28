// referenciamento.cpp
#include "mbed.h"
#include "referenciamento.h"

// funções de passo
extern void step_x(int direction, int &pos);
extern void step_y(int direction, int &pos);
extern void step_z(int direction, int &pos);

// sensores e posições
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;
extern int       x_posicao, y_posicao, z_posicao;

#define BACKOFF_STEPS 10

void referenciar() {
    bool x_ref_ok = false;
    bool y_ref_ok = false;
    bool z_ref_ok = false;

    // roda até X, Y e Z estarem referenciados
    while (!(x_ref_ok && y_ref_ok && z_ref_ok)) {

        // — EIXO X —
        if (!x_ref_ok) {
            if (xMin.read() == 1) {
                // anda negativo até tocar o fim de curso min
                step_x(-1, x_posicao);
            } else {
                // back‐off assim que tocar
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_x(+1, x_posicao);
                }
                x_posicao = 0;
                x_ref_ok  = true;
            }
        }

        // — EIXO Y —
        if (!y_ref_ok) {
            if (yMin.read() == 1) {
                step_y(-1, y_posicao);
            } else {
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_y(+1, y_posicao);
                }
                y_posicao = 0;
                y_ref_ok  = true;
            }
        }

        // — EIXO Z —
        if (!z_ref_ok) {
            if (zMin.read() == 1) {
                step_z(-1, z_posicao);
            } else {
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_z(+1, z_posicao);
                }
                z_posicao = 0;
                z_ref_ok  = true;
            }
        }
    }
}
