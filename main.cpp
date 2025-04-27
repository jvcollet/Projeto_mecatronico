// main.cpp
#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"
#include "nextion_interface.h"

// Definições de drivers de passo para X e Y
DigitalOut DIR_X(D6);
DigitalOut CLK_X(D12);
DigitalOut ENABLE_X(D5, 0); // enable ativo baixo
DigitalOut DIR_Y(D7);
DigitalOut CLK_Y(D11);
DigitalOut ENABLE_Y(D4, 0); // enable ativo baixo

// Motor Z via BusOut
BusOut MP3(D12, D13, D14, D15);

// Sensores de fim de curso
DigitalIn xMin(D9);
DigitalIn xMax(D10);
DigitalIn yMin(D2);
DigitalIn yMax(D3);
DigitalIn zMin(D10);
DigitalIn zMax(D11);

// Contadores de passos
int x_posicao = 0;
int y_posicao = 0;
int z_posicao = 0;

// Controle geral
DigitalOut Led(LED1);
DigitalIn  botao(PC_13);
DigitalIn  emergencia(PC_14);
AnalogIn   xAxis(A0);
AnalogIn   yAxis(A1);

// Serial Nextion definido em nextion_interface.cpp
extern Serial nextion;

int main() {
    bool manual = true;

    // Referenciamento fora do loop para testes
    referenciar();

    while (true) {
        // // 1) Trigger de referenciamento via Nextion ('R')
        // if (nextion.readable() && nextion.getc() == 'R') {
        //     printf("\nReferenciando...\n");
        //     referenciar();
        // }

        // 2) Tratamento de emergência (NR-12)
        if (emergencia.read() == 0) {
            ENABLE_X = 1;
            ENABLE_Y = 1;
            MP3 = 0;
            Led = 0;
            printf("\n!!! EMERGÊNCIA ATIVADA - Movimentos suspensos !!!\n");
            while (emergencia.read() == 0) wait_ms(100);
            printf("\nEmergência liberada, referenciando...\n");
            referenciar();
        }

        // 3) Leitura de joystick
        int xv = xAxis.read() * 1000;
        int yv = yAxis.read() * 1000;
        printf("\nX=%4d  Y=%4d  Pos=(%d,%d,%d)\r\n",
               xv, yv, x_posicao, y_posicao, z_posicao);

        // 4) Movimento manual (X/Y joystick + Z Nextion)
        movimento_manual(xv, yv, manual);
    }
}
