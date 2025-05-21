#include "mbed.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"
#include "pipetadora.h"

// Funções auxiliares para movimentação separada de eixos
static void mover_z_apenas(int targetZ) {
    const int delay_us = 200;
    int dz = targetZ - z_posicao;
    int dir_z = (dz > 0) ? +1 : -1;
    dz = abs(dz);
    for (int i = 0; i < dz; i++) {
        step_z(dir_z, z_posicao);
        wait_us(delay_us);
    }
}

static void mover_xy_apenas(int targetX, int targetY) {
    const int delay_us = 200;
    int dx = targetX - x_posicao;
    int dy = targetY - y_posicao;
    int dir_x = (dx > 0) ? +1 : -1;
    int dir_y = (dy > 0) ? +1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    int passos_totais = (dx > dy) ? dx : dy;
    int erro_x = 0, erro_y = 0;
    for (int i = 0; i < passos_totais; i++) {
        if (dx > 0) {
            erro_x += dx;
            if (erro_x >= passos_totais) {
                step_x(dir_x, x_posicao);
                erro_x -= passos_totais;
            }
        }
        if (dy > 0) {
            erro_y += dy;
            if (erro_y >= passos_totais) {
                step_y(dir_y, y_posicao);
                erro_y -= passos_totais;
            }
        }
        wait_us(delay_us);
    }
}

// Função principal: move Z ao zero antes de XY, depois desce Z ao alvo
static void mover_para_posicao(const Posicao &alvo) {
    // 1) Elevar Z para posição zero (altura segura)
    mover_z_apenas(0);

    // 2) Movimentar apenas X e Y até as coordenadas alvo
    mover_xy_apenas(alvo.x, alvo.y);

    // 3) Descer Z até a profundidade alvo
    mover_z_apenas(alvo.z);
}

// Restante do código permanece inalterado

// Salva uma nova posição
void salvar_posicao(float x_atual, float y_atual, float z_atual) {
    if (estado_interface == 2 && !coleta_salva) {
        posicao_coleta.x = x_atual;
        posicao_coleta.y = y_atual;
        posicao_coleta.z = z_atual;
        coleta_salva = true;
        // Atualiza Nextion ao clicar OK
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
            // Atualiza Nextion ao clicar OK
            atualizar_t0("Selecione a posicao e o volume e precione em OK");
            char texto_1[32];
            sprintf(texto_1, "Posicao %d - %dml", posicao_index + 1, volume_atual);
            atualizar_t1(texto_1);
        } else {
            // Atualiza Nextion ao clicar OK
            atualizar_t0("Ciclo OK para iniciar o sistema");
            atualizar_t1("Pressione OK para executar");
            pronto_iniciar = true;
        }
    }
}

void logica_interface_usuario(bool iniciar_sistema, bool mais, bool menos, bool ok) {
    // ... (lógica existente permanece igual)
}

// Executa o ciclo completo: coleta e dispensa volumes
void executar_ciclo(void) {
    if (!coleta_salva) {
        atualizar_t0("Posicao de coleta nao definida.");
        return;
    }

    atualizar_t0("Executando ciclo...");
    wait_ms(1000);

    for (int i = 0; i < total_posicoes; ++i) {
        const Posicao &dispensa = posicoes[i];
        int volume_restante = dispensa.volume;

        while (volume_restante > 0) {
            char texto_1[32];
            sprintf(texto_1, "Posicao %d - %dml restante", i + 1, volume_restante);
            atualizar_t1(texto_1);

            mover_para_posicao(posicao_coleta);
            wait_ms(500);

            acionar_coleta();
            wait_ms(500);

            mover_para_posicao(dispensa);
            wait_ms(500);

            acionar_dispensa();
            wait_ms(500);

            volume_restante--;
        }
    }

    atualizar_t0("Ciclo finalizado.");
}
