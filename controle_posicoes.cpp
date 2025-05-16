#include "mbed.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"
#include "pipetadora.h"

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
            atualizar_t0("Ciclo OK para iniciar o sistema");
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

            if (ok) {
                if (!pronto_iniciar) {
                    salvar_posicao(x_posicao, y_posicao, z_posicao);
                } else {
                    atualizar_t0("Iniciando ciclo...");
                    wait_us(500);
                    executar_ciclo();

                    // Reset após o ciclo
                    estado_interface = 0;
                    total_dispensas = 1;
                    volume_atual = 1;
                    posicao_index = 0;
                    total_posicoes = 0;
                    coleta_salva = false;
                    pronto_iniciar = false;

                    atualizar_t0("Clique iniciar para novo ciclo");
                    atualizar_t1("Aguardando comando...");
                }
            }
            break;

        default:
            break;
    }
}

// Função auxiliar: move rapidamente até a posição alvo
static void mover_para_posicao(const Posicao &alvo) {
    const int delay_us = 200;

    int dx = alvo.x - x_posicao;
    int dy = alvo.y - y_posicao;

    int dir_x = (dx > 0) ? +1 : -1;
    int dir_y = (dy > 0) ? +1 : -1;
    dx = abs(dx);
    dy = abs(dy);

    int passos_x = dx;
    int passos_y = dy;

    int passos_totais = (dx > dy) ? dx : dy;

    int erro_x = 0;
    int erro_y = 0;

    for (int i = 0; i < passos_totais; i++) {
        if (passos_x > 0) {
            erro_x += dx;
            if (erro_x >= passos_totais) {
                step_x(dir_x, x_posicao);
                erro_x -= passos_totais;
            }
        }

        if (passos_y > 0) {
            erro_y += dy;
            if (erro_y >= passos_totais) {
                step_y(dir_y, y_posicao);
                erro_y -= passos_totais;
            }
        }

        // Atualiza display a cada N passos para evitar lentidão
        // if (i % 50 == 0) {
        //     char buffer[64];
        //     sprintf(buffer, "Posicao- X: %d Y: %d", x_posicao, y_posicao);
        //     atualizar_t2(buffer);
        // }
    }
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
