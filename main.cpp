#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"

BusOut    MP1(D3, D4, D5, D6);
BusOut    MP2(D8, D9, D10, D11);
BusOut    MP3(D12, D13, D14, D15);
DigitalOut Led(LED1);
DigitalIn  botao(PC_13);
AnalogIn   xAxis(A0);
AnalogIn   yAxis(A1);

// Sensores e posições vindos de referenciamento.cpp
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;
extern int x_posicao, y_posicao, z_posicao;

int main() {
    bool estado = false;

    // 1) Executa homing para zerar as posições
    referenciar();

    // 2) Loop principal
    while (true) {
        int x_val = xAxis.read() * 1000;
        int y_val = yAxis.read() * 1000;

        printf("\nX=%4d  Y=%4d  Pos=(%d,%d)", 
               x_val, y_val, x_posicao, y_posicao);

        movimento_joystick(x_val, y_val);
    }
}
