// Inclui as bibliotecas
#include "mbed.h"
#include "movimento.h"


// Instâncias globais
BusOut MP1(D3, D4, D5, D6);
BusOut MP2(D8, D9, D10, D11);

DigitalOut Led(LED1);
DigitalIn botao(PC_13);
AnalogIn xAxis(A0);
AnalogIn yAxis(A1);

int main() {
    bool estado = 0;
    int x_joystick, y_joystick;

    Led = 0;

    while (1) {
        x_joystick = xAxis.read() * 1000;
        y_joystick = yAxis.read() * 1000;

        printf("\nX=%4d\tY=%4d", x_joystick, y_joystick);
        Led = estado;

        movimento_joystick(x_joystick, y_joystick);

        if (botao == 0) {
            estado = !estado;
            wait(0.3);
        }
    }
}
