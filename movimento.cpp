#include "mbed.h" // Inclui mbed

// Referência às variáveis definidas em main.cpp
extern BusOut MP1;
extern BusOut MP2;

#define velo 0.01

// Função de movimentação com joystick
void movimento_joystick(int x_joystick, int y_joystick) {
    if (x_joystick > 550) {
        for (int i = 0; i < 4; i++) {
            MP1 = 1 << i;
            wait(velo);
        }
    }

    if (x_joystick < 450) {
        for (int i = 3; i >= 0; i--) {
            MP1 = 1 << i;
            wait(velo);
        }
    }

    if (y_joystick > 550) {
        for (int i = 0; i < 4; i++) {
            MP2 = 1 << i;
            wait(velo);
        }
    }

    if (y_joystick < 450) {
        for (int i = 3; i >= 0; i--) {
            MP2 = 1 << i;
            wait(velo);
        }
    }
}
