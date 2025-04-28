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
DigitalIn xMax(D9);
DigitalIn yMin(D2);
DigitalIn yMax(D3);
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

int main() {

     // ------------------------------------------------
    // A) Ativa pull-ups internos nos sensores (estabiliza leitura)
    // ------------------------------------------------

    xMin.mode(PullUp);  xMax.mode(PullUp);
    yMin.mode(PullUp);  yMax.mode(PullUp);
    zMin.mode(PullUp);  zMax.mode(PullUp);

    // 1) Referenciamento inicial
    referenciar();
    Led = 1;  // LED indica que estamos prontos


    bool modo_manual = true;  // Controle se estamos em modo de movimentação manual
    int contador_posicoes = 0;

    printf("\n=== Sistema Iniciado ===\n");



    // 2) Posicionar e salvar locais de coleta e dispensação
    printf("\n>> Posicione a pipeta na posição de COLETA usando o joystick.\n");
    printf(">> Pressione o botão para salvar a posição de coleta.\n");

    while (contador_posicoes == 0) {
        // Leitura do joystick
        int xv = xAxis.read() * 1000;
        int yv = yAxis.read() * 1000;
        movimento_manual(xv, yv, modo_manual);

        // Quando o botão for pressionado, salva a posição de coleta
        if (botao.read() == 0) {
            salvar_posicao(x_posicao, y_posicao, z_posicao);
            contador_posicoes++;
            wait(0.5);  // Debounce simples
        }

        // Tratamento de botão de emergência
        if (emergencia.read() == 0) {
            printf("\n!!! Emergência !!!\n");
            ENABLE_X = 1;
            ENABLE_Y = 1;
            MP3 = 0;
            Led = 0;
            while (emergencia.read() == 0) wait(0.1);
            printf("\nEmergência liberada. Referenciando...\n");
            referenciar();
            Led = 1;
        }
    }

    printf("\n>> Agora posicione a pipeta nas posições de DISPENSA usando o joystick.\n");
    printf(">> Pressione o botão para salvar cada posição de dispensa.\n");

    while (contador_posicoes < 10) { // 1 coleta + 9 dispensas = 10 posições
        int xv = xAxis.read() * 1000;
        int yv = yAxis.read() * 1000;
        movimento_manual(xv, yv, modo_manual);

        if (botao.read() == 0) {
            salvar_posicao(x_posicao, y_posicao, z_posicao);
            contador_posicoes++;
            printf("\nPosição %d salva.\n", contador_posicoes - 1);
            wait(0.5);  // Debounce
        }

        if (emergencia.read() == 0) {
            printf("\n!!! Emergência !!!\n");
            ENABLE_X = 1;
            ENABLE_Y = 1;
            MP3 = 0;
            Led = 0;
            while (emergencia.read() == 0) wait(0.1);
            printf("\nEmergência liberada. Referenciando...\n");
            referenciar();
            Led = 1;
        }
    }

    // 3) Tudo posicionado! Agora começa o ciclo automático
    printf("\n>> Iniciando ciclo automático de coleta e dispensação...\n");
    wait(2.0);

    while (true) {
        executar_ciclo();  // Função que faz todo o ciclo de coleta/dispensação
        printf("\n>> Ciclo finalizado. Aguardando 5 segundos para reiniciar...\n");
        wait(5.0);
    }
}
