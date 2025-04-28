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
    bool x_ref_ok = false;
    bool y_ref_ok = false;
    bool z_ref_ok = false;

    while (!(x_ref_ok)) {  // Enquanto nem todos referenciados
        // Movimento eixo X
        if (!x_ref_ok) {
            if (xMax.read() == 1) {  // Sensor NÃO acionado
                step_x(1, x_posicao);
            } else {  // Sensor ACIONADO
                // FAZ O BACKOFF IMEDIATAMENTE
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_x(-1, x_posicao); // Recuo no sentido contrário
                }
                x_posicao = 0;
                x_ref_ok = true;  // Agora marcou como referenciado
            }
        }

        // Movimento eixo Y
        if (!y_ref_ok) {
            if (yMax.read() == 1) {  // Sensor NÃO acionado
                step_y(1, y_posicao);
            } else {  // Sensor ACIONADO
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_y(-1, y_posicao);
                }
                y_posicao = 0;
                y_ref_ok = true;
            }
        }

        // Movimento eixo Z
        if (!z_ref_ok) {
            if (zMax.read() == 1) {  // Sensor NÃO acionado
                step_z(1, z_posicao);
            } else {  // Sensor ACIONADO
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_z(-1, z_posicao);
                }
                z_posicao = 0;
                z_ref_ok = true;
            }
        }
    }
}