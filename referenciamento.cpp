#include "mbed.h"
#include "referenciamento.h"
#include "nextion_interface.h"
#include "movimento.h"

// Funções de passo
extern void step_x(int direction, int &pos);
extern void step_y(int direction, int &pos);
extern void step_z(int direction, int &pos);

// Sensores e posições
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;
extern int       x_posicao, y_posicao, z_posicao;

#define BACKOFF_STEPS   2000
#define STEP_DELAY_US   200
#define WAIT_BACKOFF_MS 500
#define BACKOFF_STEPS_Z   150


Timer timer_x;
Timer timer_y;
Timer timer_z;

void referenciar() {
    bool x_ref_ok = false, y_ref_ok = false, z_ref_ok = false;
    bool x_backoff_pending = false, y_backoff_pending = false, z_backoff_pending = false;

    timer_x.start();
    timer_y.start();
    timer_z.start();

    while (!(x_ref_ok && y_ref_ok && z_ref_ok)) {
        // — EIXO X —
        if (!x_ref_ok && z_ref_ok) {
            if (!x_backoff_pending) {
                if (xMax.read() == 1) {
                    step_x(+1, x_posicao);
                    wait_us(STEP_DELAY_US);
                } else {
                    x_backoff_pending = true;
                    timer_x.reset();
                }
            } else if (timer_x.read_ms() >= WAIT_BACKOFF_MS) {
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_x(-1, x_posicao);
                    wait_us(STEP_DELAY_US);
                }
                x_posicao = 0;
                x_ref_ok = true;
            }
        }

        // — EIXO Y —
        if (!y_ref_ok && z_ref_ok) {
            if (!y_backoff_pending) {
                if (yMax.read() == 1) {
                    step_y(+1, y_posicao);
                    wait_us(STEP_DELAY_US);
                } else {
                    y_backoff_pending = true;
                    timer_y.reset();
                }
            } else if (timer_y.read_ms() >= WAIT_BACKOFF_MS) {
                for (int i = 0; i < BACKOFF_STEPS; ++i) {
                    step_y(-1, y_posicao);
                    wait_us(STEP_DELAY_US);
                }
                y_posicao = 0;
                y_ref_ok = true;
            }
        }

        // — EIXO Z —
        if (!z_ref_ok) {
            if (!z_backoff_pending) {
                if (zMin.read() == 1) {
                    step_z(-1, z_posicao);
                    wait_us(STEP_DELAY_US);
                } else {
                    z_backoff_pending = true;
                    timer_z.reset();
                }
            } else if (timer_z.read_ms() >= WAIT_BACKOFF_MS) {
                for (int i = 0; i < BACKOFF_STEPS_Z; ++i) {
                    step_z(+1, z_posicao);
                    wait_us(VELO_HOMING_Z);
                }
                z_posicao = 0;
                z_ref_ok = true;
            }
        }
    }
}
