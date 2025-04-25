#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"
#include "nextion_interface.h"  // Comunicação com Nextion

BusOut    MP1(D3, D4, D5, D6);
BusOut    MP2(D8, D9, D10, D11);
BusOut    MP3(D12, D13, D14, D15);
DigitalOut Led(LED1);
DigitalIn  emergencia(PC_14);  // Botão de emergência, ativo em 0
DigitalIn  botao(PC_13);
AnalogIn   xAxis(A0);
AnalogIn   yAxis(A1);

// Serial do Nextion: TX=P A_9, RX=PA_10; ajuste se necessário
Serial    nextion(PA_9, PA_10, 9600);

// Sensores e posições vindos de referenciamento.cpp
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;
extern int x_posicao, y_posicao, z_posicao;

int main() {
    bool estado = false;

    // 1) Executa homing para zerar as posições
    // Temos que adicionar um if se for clicado
    referenciar();

    // 2) Loop principal
    while (true) {
        // ... Referenciamento por botão no Nextion
        if (nextion.readable()) {
            char cmd = nextion.getc();
            if (cmd == 'R') {
                printf("\nExecutando referenciamento...\n");
                referenciar();
            }
        }
        // Se é clicado no botão de emergência
        if (emergencia.read() == 0) {
            // Desliga todos os motores e sinaliza estado de emergência
            MP1 = 0;
            MP2 = 0;
            MP3 = 0;
            Led = 0;
            printf("\n!!! EMERGENCIA ATIVADA - Movimentos suspensos !!!\n");
            // Aguarda liberação do botão de emergência
            while (emergencia.read() == 0) {
                MP1 = 0;
                MP2 = 0;
                MP3 = 0;
                Led = 0;
                wait(200);
            }
            printf("\nEmergencia liberada, retomando operacao...\n");
            // Reposiciona homing para segurança
            referenciar(); // Temos que adicionar uma questão para solicitar o referenciamento
        }

        // Leitura do joystick
        int x_val = xAxis.read() * 1000;
        int y_val = yAxis.read() * 1000;

        printf("\nX=%4d  Y=%4d  Pos=(%d,%d)", 
               x_val, y_val, x_posicao, y_posicao);
        // Movimento do joystick
        movimento_joystick(x_val, y_val);
    }
}
