#include "mbed.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"
#include "pipetadora.h"
#include "movimento.h"
extern DigitalOut buzzer;



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
    switch (estado_interface) {
        case 0:
            if (iniciar_sistema) {
                // Atualiza Nextion ao clicar iniciar
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
                // Atualiza Nextion ao clicar + ou -
                char texto_1[32];
                sprintf(texto_1, "Numero de dispensas: %d", total_dispensas);
                atualizar_t1(texto_1);
            }
            if (ok) {
                // Atualiza Nextion ao clicar OK
                atualizar_t0("Selecione a posicao de coleta");
                atualizar_t1("Clique em OK na posicao");
                estado_interface = 2;
            }
            break;

        case 2:
            if (ok) salvar_posicao(x_posicao, y_posicao, z_posicao);
            break;

        case 3:
            if (mais) {
                volume_atual++;
                // Atualiza Nextion ao clicar +
                char texto_1[32];
                sprintf(texto_1, "Posicao %d - %dml", posicao_index + 1, volume_atual);
                atualizar_t1(texto_1);
            }
            if (menos && volume_atual > 1) {
                volume_atual--;
                // Atualiza Nextion ao clicar -
                char texto_1[32];
                sprintf(texto_1, "Posicao %d - %dml", posicao_index + 1, volume_atual);
                atualizar_t1(texto_1);
            }

            if (ok) {
                if (!pronto_iniciar) {
                    salvar_posicao(x_posicao, y_posicao, z_posicao);
                } else {
                    // Atualiza Nextion ao clicar OK para iniciar ciclo
                    atualizar_t0("Iniciando ciclo...");
                    wait_us(500);
                    executar_ciclo(iniciar_sistema);

                    // Reset após o ciclo
                    estado_interface = 0;
                    total_dispensas = 1;
                    volume_atual = 1;
                    posicao_index = 0;
                    total_posicoes = 0;
                    coleta_salva = false;
                    pronto_iniciar = false;

                    // Atualiza Nextion após ciclo
                    atualizar_t0("Clique iniciar para novo ciclo");
                    atualizar_t1("Aguardando comando...");
                }
            }
            break;

        default:
            break;
    }
}

static void mover_para_posicao(const Posicao &alvo) {
    const int delay_us = 100;

    // 1) Elevar Z até o topo (fim de curso superior)
    while (z_posicao < 0 && zMin.read() == 1) {
        step_z(-1, z_posicao);
        wait_us(delay_us);
    }

    // 2) Deslocar X e Y em Bresenham (mantendo Z no topo)
    int dx = abs(alvo.x - x_posicao);
    int dy = abs(alvo.y - y_posicao);
    int dir_x = (alvo.x > x_posicao) ? +1 : -1;
    int dir_y = (alvo.y > y_posicao) ? +1 : -1;

    int passos_xy = dx > dy ? dx : dy;
    int erro_x = 0, erro_y = 0;

    for (int i = 0; i < passos_xy; i++) {
        if (dx > 0) {
            erro_x += dx;
            if (erro_x >= passos_xy) {
                step_x(dir_x, x_posicao);
                erro_x -= passos_xy;
            }
        }
        if (dy > 0) {
            erro_y += dy;
            if (erro_y >= passos_xy) {
                step_y(dir_y, y_posicao);
                erro_y -= passos_xy;
            }
        }
        wait_us(delay_us);
    }

    // 3) Descer Z até a altura desejada
    while (z_posicao > alvo.z && zMin.read()) {
        step_z(+1, z_posicao);
        wait_us(delay_us);
    }
}


// Executa o ciclo completo: coleta e dispensa volumes
void executar_ciclo(bool btn_iniciar) {
    const int delay_us = 200;
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
            wait_ms(1000);

            acionar_coleta();
            wait_ms(1000);

            mover_para_posicao(dispensa);
            wait_ms(1000);

            acionar_dispensa();
            wait_ms(1000);

            volume_restante--;
        }
    }
    
    while (z_posicao < 0 && zMin.read() == 1) {
        step_z(-1, z_posicao);
        wait_us(delay_us);
    }

    atualizar_t0("Ciclo finalizado.");
    buzzer = 1;
    wait_ms(3000); // tempo para o operador ver o alerta
    buzzer = 0;

    // Reset apenas das variáveis de controle
    estado_interface = 0;
    total_dispensas = 1;
    volume_atual = 1;
    posicao_index = 0;
    total_posicoes = 0;
    coleta_salva = false;
    pronto_iniciar = false;

    // Atualiza display informando que novo ciclo pode ser iniciado
    atualizar_t0("Clique iniciar para novo ciclo");
    atualizar_t1("Aguardando comando...");
}