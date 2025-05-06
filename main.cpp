// === main.cpp ===
#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"

// Drivers de passo para X e Y
DigitalOut DIR_X(D7);
DigitalOut CLK_X(D11);
DigitalOut ENABLE_X(D4, 0);  // enable ativo baixo
DigitalOut DIR_Y(D6);
DigitalOut CLK_Y(D12);
DigitalOut ENABLE_Y(D5, 0);  // enable ativo baixo

// Motor Z via BusOut
BusOut MP3(D13, D14, D15, D15);

// Sensores de fim de curso
DigitalIn xMin(D10);
DigitalIn xMax(D3);
DigitalIn yMin(D2);
DigitalIn yMax(D9);
DigitalIn zMin(D0);
DigitalIn zMax(D1);

// Contadores de posição
int x_posicao = 0;
int y_posicao = 0;
int z_posicao = 0;

// Controles adicionais
DigitalOut Led(LED1);
DigitalIn  botao(PC_13);
DigitalIn  emergencia(PC_14);
AnalogIn   xAxis(A0);
AnalogIn   yAxis(A1);

// Comunicação com Nextion (declarado em nextion_interface.cpp)
extern Serial nextion;
extern char comando_atual[10];


int main() {
    iniciar_nextion();

    // Ativa pull-ups internos nos sensores
    xMin.mode(PullUp);  xMax.mode(PullUp);
    yMin.mode(PullUp);  yMax.mode(PullUp);
    zMin.mode(PullUp);  zMax.mode(PullUp);

    // Variáveis de controle dos botões
    bool salvar_coleta = false;
    bool salvar_dispensa = false;
    bool iniciar_ciclo = false;
    bool referenciar_sistema = false;
    bool aumentar_volume = false;
    bool diminuir_volume = false;

    // Mensagens iniciais
    atualizar_t0("Sistema iniciado");
    atualizar_t1("Clique no botão de referenciar");
    atualizar_t2("Posicoes ainda não refernciadas");

    bool modo_manual = true;
    int contador_posicoes = 0;

    while (true) {
        int xv = xAxis.read() * 1000;
        int yv = yAxis.read() * 1000;
        movimento_manual(xv, yv, modo_manual);
        
        atualizar_comando();

        botao_referenciamento(referenciar_sistema);

        if (referenciar_sistema) {
            atualizar_t0("Referenciando...");
            atualizar_t1("Aguarde");
            referenciar();
            atualizar_t0("Sistema pronto");
            atualizar_t1("Use o joystick para posicionar coleta");
        
            referenciar_sistema = false;
            strcpy(comando_atual, ""); // LIMPA O COMANDO AQUI
        }
        wait_ms(100);
    }
    }
//         if (botao.read() == 0) {
//             salvar_posicao(x_posicao, y_posicao, z_posicao);
//             contador_posicoes++;
//             atualizar_t0("Posição de coleta salva");
//             atualizar_t1("Posicione para dispensa");
//             wait(0.5);
//             break;
//         }

//         if (emergencia.read() == 0) {
//             printf("\n!!! Emergência !!!\n");
//             atualizar_t0("!!! Emergência !!!");
//             atualizar_t1("Aguardando liberação...");
//             ENABLE_X = 1;
//             ENABLE_Y = 1;
//             MP3 = 0;
//             Led = 0;
//             while (emergencia.read() == 0) wait(0.1);
//             // printf("\nEmergência liberada. Referenciando...\n");
//             atualizar_t0("Referenciando...");
//             atualizar_t1("Aguarde");
//             referenciar();
//             Led = 1;
//             atualizar_t0("Sistema pronto");
//         }
//     }

    
//     while (contador_posicoes < 10) {
//         int xv = xAxis.read() * 1000;
//         int yv = yAxis.read() * 1000;
//         movimento_manual(xv, yv, modo_manual);

//         if (botao.read() == 0) {
//             salvar_posicao(x_posicao, y_posicao, z_posicao);
//             contador_posicoes++;
//             char msg[32];
//             // sprintf(msg, "Dispensa %d salva", contador_posicoes - 1);
//             // printf("\n%s\n", msg);
//             atualizar_t0(msg);
//             wait(0.5);
//         }

//         if (emergencia.read() == 0) {
//             atualizar_t0("!!! Emergência !!!");
//             atualizar_t1("Aguardando liberação...");
//             ENABLE_X = 1;
//             ENABLE_Y = 1;
//             MP3 = 0;
//             Led = 0;
//             while (emergencia.read() == 0) wait(0.1);
//             atualizar_t0("Referenciando...");
//             atualizar_t1("Aguarde");
//             referenciar();
//             Led = 1;
//             atualizar_t0("Sistema pronto");
//         }
//     }

//     atualizar_t0("Executando ciclo...");
//     atualizar_t1("Aguarde");
//     wait(2.0);

//     while (true) {
//         executar_ciclo();
//         atualizar_t0("Ciclo finalizado");
//         atualizar_t1("Reiniciando em 5s...");
//         wait(5.0);
//     }
// }

