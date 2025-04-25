#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"
#include "nextion_interface.h"
#include "controle_posicoes.h"   // NOVO: para salvar e executar posições

BusOut    MP1(D3, D4, D5, D6);
BusOut    MP2(D8, D9, D10, D11);
BusOut    MP3(D12, D13, D14, D15);
DigitalOut Led(LED1);
DigitalIn  emergencia(PC_14);  
DigitalIn  botao(PC_13);
AnalogIn   xAxis(A0);
AnalogIn   yAxis(A1);

// Serial do Nextion
Serial    nextion(PA_9, PA_10, 9600);

// Sensores e posições vindos de referenciamento.cpp
extern DigitalIn xMin, xMax, yMin, yMax, zMin, zMax;
extern int x_posicao, y_posicao, z_posicao;

int main() {
    printf("\nSistema Iniciado.\n");
    
    // 1) Executa homing para zerar as posições (se quiser manual, com comando 'R')
    // referenciar();

    while (true) {
        // -- BOTÃO DE EMERGÊNCIA --
        if (emergencia.read() == 0) {
            MP1 = MP2 = MP3 = 0;
            Led = 0;
            printf("\n!!! EMERGÊNCIA ATIVADA !!!\n");
            while (emergencia.read() == 0) {
                MP1 = MP2 = MP3 = 0;
                Led = 0;
                wait(0.2f);
            }
            printf("\nEmergência liberada. Executando novo referenciamento.\n");
            referenciar();
        }

        // -- COMANDOS DO NEXTION --
        if (nextion.readable()) {
            char cmd = nextion.getc();

            if (cmd == 'R') {   // Referenciar
                printf("\nExecutando referenciamento...\n");
                referenciar();
            }
            else if (cmd == 'C') {  // Salvar posição de coleta
                salvar_posicao(x_posicao, y_posicao, z_posicao);
            }
            else if (cmd == 'D') {  // Salvar posição de dispensa (opcional, pode usar 'C' pra todos)
                salvar_posicao(x_posicao, y_posicao, z_posicao);
            }
            else if (cmd == 'A') {  // Iniciar ciclo automático
                executar_ciclo();
            }
        }

        // -- CONTROLE MANUAL PELO JOYSTICK --
        int x_val = xAxis.read() * 1000;
        int y_val = yAxis.read() * 1000;

        printf("\nJoystick - X: %4d  Y: %4d | Posição - (%d, %d)", 
               x_val, y_val, x_posicao, y_posicao);

        movimento_joystick(x_val, y_val);
    }
}