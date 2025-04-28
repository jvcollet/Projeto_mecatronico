// === controle_posicoes.cpp ===
#include "mbed.h"
#include "controle_posicoes.h"

// Variáveis de posição globais
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

// Lista de posições salvas
static Posicao posicoes[MAX_POSICOES];
static int total_posicoes = 0;

// Salva uma nova posição
void salvar_posicao(float x_atual, float y_atual, float z_atual) {
    if (total_posicoes < MAX_POSICOES) {
        posicoes[total_posicoes].x = x_atual;
        posicoes[total_posicoes].y = y_atual;
        posicoes[total_posicoes].z = z_atual;
        printf("\nPosição %d salva: (%.2f, %.2f, %.2f)\n",
               total_posicoes,
               posicoes[total_posicoes].x,
               posicoes[total_posicoes].y,
               posicoes[total_posicoes].z);
        total_posicoes++;
    } else {
        printf("\nLimite máximo de posições atingido!\n");
    }
}

// NOVA função auxiliar: move com segurança
static void mover_para_posicao(const Posicao &alvo) {
    // 1. Primeiro, sobe o eixo Z até o máximo (enquanto o sensor não detectar)
    printf("\nSubindo Z até o máximo...\n");
    while (zMax.read() == 1) {
        step_z(+1, z_posicao);
    }

    // 2. Move os eixos X e Y até a posição alvo
    printf("Movendo X e Y até posição desejada...\n");
    while (x_posicao < alvo.x - 1.0f) step_x(+1, x_posicao);
    while (x_posicao > alvo.x + 1.0f) step_x(-1, x_posicao);

    while (y_posicao < alvo.y - 1.0f) step_y(+1, y_posicao);
    while (y_posicao > alvo.y + 1.0f) step_y(-1, y_posicao);

    // 3. Desce o eixo Z até a posição alvo
    printf("Descendo Z até posição desejada...\n");
    while (z_posicao < alvo.z - 1.0f) step_z(+1, z_posicao);
    while (z_posicao > alvo.z + 1.0f) step_z(-1, z_posicao);
}

// Executa ciclo completo
void executar_ciclo(void) {
    printf("\nIniciando ciclo automático...\n");
    for (int i = 0; i < total_posicoes; ++i) {
        printf("\nMovendo para posição %d: (%.2f, %.2f, %.2f)\n",
               i,
               posicoes[i].x,
               posicoes[i].y,
               posicoes[i].z);
        mover_para_posicao(posicoes[i]);
        wait(1.5f);  // tempo de coleta/dispensa
    }
    printf("\nCiclo automático finalizado.\n");
}
