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

// Contadores de posicao
int x_posicao = 0;
int y_posicao = 0;
int z_posicao = 0;

// Controles adicionais
DigitalOut Led(LED1);
DigitalIn  botao(PC_13);
DigitalIn  emergencia(PC_14);
AnalogIn   xAxis(A0);
AnalogIn   yAxis(A1);

// Comunicacao com Nextion (declarado em nextion_interface.cpp)
extern Serial nextion;
extern char comando_atual[10];


int main() {
    iniciar_nextion();

    // Ativa pull-ups internos nos sensores
    xMin.mode(PullUp);  xMax.mode(PullUp);
    yMin.mode(PullUp);  yMax.mode(PullUp);
    zMin.mode(PullUp);  zMax.mode(PullUp);

    // Estados de botões Nextion
    bool btn_iniciar = false;
    bool btn_ok = false;
    bool btn_mais = false;
    bool btn_menos = false;
    bool referenciar_sistema = false;

    // Mensagens iniciais
    atualizar_t0("Clique no botao de referenciar");
    atualizar_t1("Sistema iniciado");
    atualizar_t2("Posicoes ainda nao refernciadas");

    bool modo_manual = true; 

    while (true) {
       
        botao_referenciamento(referenciar_sistema);
        botao_mais(btn_mais);
        botao_menos(btn_menos);
        botao_iniciar_sistema(btn_iniciar);
        botao_ok(btn_ok);
        

        int xv = xAxis.read() * 1000;
        int yv = yAxis.read() * 1000;

        movimento_manual(xv, yv, modo_manual);
 
        
        if (referenciar_sistema) {
            atualizar_t0("Aguarde o refernciamento");
            atualizar_t1("Referenciando...");
            referenciar();            

            atualizar_t0("Utilize o joystick para mover ou inicie o sistema");
            atualizar_t1("Aguardando comando");
        
            referenciar_sistema = false;
            strcpy(comando_atual, ""); // LIMPA O COMANDO AQUI
        }
        
        // Formato para colocar a posição do X e Y
        char buffer[64];
        sprintf(buffer, "Posicao- X: %d Y: %d", x_posicao, y_posicao);
        atualizar_t2(buffer);

        // Lógica da interface (controle_posicoes.cpp)
        logica_interface_usuario(btn_iniciar, btn_mais, btn_menos, btn_ok);
    
        // if (botao.read() == 0) {
        //     salvar_posicao(x_posicao, y_posicao, z_posicao);
        //     contador_posicoes++;
        //     atualizar_t0("Posicao de coleta salva");
        //     atualizar_t1("Posicione para dispensa");
        //     wait(0.5);
        //     break;
        // }

        // if (emergencia.read() == 0) {
        //     printf("\n!!! Emergência !!!\n");
        //     atualizar_t0("!!! Emergência !!!");
        //     atualizar_t1("Aguardando liberacao...");
        //     ENABLE_X = 1;
        //     ENABLE_Y = 1;
        //     MP3 = 0;
        //     Led = 0;
        //     while (emergencia.read() == 0) wait(0.1);
        //     // printf("\nEmergência liberada. Referenciando...\n");
        //     atualizar_t0("Referenciando...");
        //     atualizar_t1("Aguarde");
        //     referenciar();
        //     Led = 1;
        //     atualizar_t0("Sistema pronto");
        // }
    }
    }

    
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
//             atualizar_t1("Aguardando liberaçao...");
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