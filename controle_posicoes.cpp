// === controle_posicoes.cpp ===
#include "mbed.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"

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
            pronto_iniciar = true;
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

// Função auxiliar: move com segurança até a posição alvo
static void mover_para_posicao(const Posicao &alvo) {
    // 1. Sobe o eixo Z até o máximo (por segurança)
    // while (zMax.read() == 1) {
    //     step_z(+1, z_posicao);
    // }

    // 2. Move X até a posição alvo (com tolerância)
    while (x_posicao < alvo.x - 1) step_x(+1, x_posicao);
    while (x_posicao > alvo.x + 1) step_x(-1, x_posicao);

    // 3. Move Y até a posição alvo (com tolerância)
    while (y_posicao < alvo.y - 1) step_y(+1, y_posicao);
    while (y_posicao > alvo.y + 1) step_y(-1, y_posicao);

    // 4. Move Z até a posição desejada
    // while (z_posicao < alvo.z - 1) step_z(+1, z_posicao);
    // while (z_posicao > alvo.z + 1) step_z(-1, z_posicao);
}

// Executa o ciclo completo: coleta e dispensa volumes
void executar_ciclo(void) {
    if (!coleta_salva) {
        atualizar_t0("Posição de coleta não definida.");
        return;
    }

    atualizar_t0("Executando ciclo...");
    wait_ms(5000); // breve pausa para leitura da mensagem

    for (int i = 0; i < total_posicoes; ++i) {
        const Posicao &dispensa = posicoes[i];
        int volume_restante = dispensa.volume;

        while (volume_restante > 0) {
            // 1. Vai para a posição de coleta
            mover_para_posicao(posicao_coleta);
            wait_ms(5000);  // tempo simulado de coleta

            // 2. Vai para a posição de dispensa
            mover_para_posicao(dispensa);
            wait_ms(5000);  // tempo simulado de dispensa

            volume_restante--;  // cada ciclo transfere 1ml
        }
    }

    atualizar_t0("Ciclo finalizado.");
}