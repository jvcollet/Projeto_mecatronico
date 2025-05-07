// === controle_posicoes.cpp ===
#include "mbed.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"

// Variáveis de posição globais
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

// Lista de posições salvas
static Posicao posicoes[MAX_POSICOES];
static int total_posicoes = 0;

// Posição de coleta única
static Posicao posicao_coleta;
bool coleta_salva = false;

// Estado da interface
static int estado_interface = 0;
static int total_dispensas = 1;
static int volume_atual = 1;
static int posicao_index = 0;


// Salva uma nova posição
void salvar_posicao(float x_atual, float y_atual, float z_atual) {
    if (estado_interface == 2 && !coleta_salva) {
        posicao_coleta.x = x_atual;
        posicao_coleta.y = y_atual;
        posicao_coleta.z = z_atual;
        coleta_salva = true;
        atualizar_t0("Selecione a posicao e o volume e precione em OK");
        char texto_1[32];
        sprintf(texto_1, "Posicao %d - %dml", posicao_index + 1, volume_atual);
        atualizar_t1(texto_1);
        estado_interface = 3;
    } else if (estado_interface == 3 && total_posicoes < total_dispensas) {
        posicoes[total_posicoes].x = x_atual;
        posicoes[total_posicoes].y = y_atual;
        posicoes[total_posicoes].z = z_atual;
        posicoes[total_posicoes].volume = volume_atual;
        total_posicoes++;

        if (total_posicoes < total_dispensas) {
            posicao_index++;
            volume_atual = 1;
            atualizar_t0("Selecione a posicao e o volume e precione em OK");
            char texto_1[32];
            sprintf(texto_1, "Posicao %d - %dml", posicao_index + 1, volume_atual);
            atualizar_t1(texto_1);
        } else {
            atualizar_t0("Ciclo pronto para iniciar");
            atualizar_t1("Pressione OK para executar");
        }
    }
}

void logica_interface_usuario(bool iniciar_sistema, bool mais, bool menos, bool ok) {
    switch (estado_interface) {
        case 0:
            if (iniciar_sistema) {
                atualizar_t0("Selecione o numero de dispensas e clique em OK");
                char texto_1[32];
                sprintf(texto_1, "Numero de dispensas: %d", total_dispensas);
                atualizar_t1(texto_1);
                estado_interface = 1;
            }
            break;

        case 1:
            if (mais && total_dispensas < MAX_POSICOES) total_dispensas++;
            if (menos && total_dispensas > 1) total_dispensas--;
            if (mais || menos) {
                char texto_1[32];
                sprintf(texto_1, "Numero de dispensas: %d", total_dispensas);
                atualizar_t1(texto_1);
            }
            if (ok) {
                atualizar_t0("Selecione a posicao de coleta");
                atualizar_t1("Clique em OK na posicao");
                estado_interface = 2;
            }
            break;

        case 2:
            if (ok) salvar_posicao(x_posicao, y_posicao, z_posicao);
            break;

        case 3:
            if (mais) volume_atual++;
            if (menos && volume_atual > 1) volume_atual--;
            if (mais || menos) {
                char texto_1[32];
                sprintf(texto_1, "Posicao %d - %dml", posicao_index + 1, volume_atual);
                atualizar_t1(texto_1);
            }
            if (ok) salvar_posicao(x_posicao, y_posicao, z_posicao);
            break;

        default:
            break;
    }
}


// NOVA função auxiliar: move com segurança
// static void mover_para_posicao(const Posicao &alvo) {
//     // 1. Primeiro, sobe o eixo Z até o máximo (enquanto o sensor não detectar)
//     printf("\nSubindo Z até o máximo...\n");
//     while (zMax.read() == 1) {
//         step_z(+1, z_posicao);
//     }

//     // 2. Move os eixos X e Y até a posição alvo
//     printf("Movendo X e Y até posição desejada...\n");
//     while (x_posicao < alvo.x - 1.0f) step_x(+1, x_posicao);
//     while (x_posicao > alvo.x + 1.0f) step_x(-1, x_posicao);

//     while (y_posicao < alvo.y - 1.0f) step_y(+1, y_posicao);
//     while (y_posicao > alvo.y + 1.0f) step_y(-1, y_posicao);

//     // 3. Desce o eixo Z até a posição alvo
//     printf("Descendo Z até posição desejada...\n");
//     while (z_posicao < alvo.z - 1.0f) step_z(+1, z_posicao);
//     while (z_posicao > alvo.z + 1.0f) step_z(-1, z_posicao);
// }

// // Executa ciclo completo
// void executar_ciclo(void) {
//     printf("\nIniciando ciclo automático...\n");
//     for (int i = 0; i < total_posicoes; ++i) {
//         printf("\nMovendo para posição %d: (%.2f, %.2f, %.2f)\n",
//                i,
//                posicoes[i].x,
//                posicoes[i].y,
//                posicoes[i].z);
//         mover_para_posicao(posicoes[i]);
//         wait(1.5f);  // tempo de coleta/dispensa
//     }
//     printf("\nCiclo automático finalizado.\n");
// }