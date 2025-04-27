// controle_posicoes.h
#ifndef CONTROLE_POSICOES_H
#define CONTROLE_POSICOES_H

#include "mbed.h"
#include "movimento.h"  // para protótipos de step_x, step_y, step_z

// Número máximo de posições
#define MAX_POSICOES 9

// Estrutura para armazenar uma posição
typedef struct {
    float x;
    float y;
    float z;
} Posicao;

// Funções disponíveis
void salvar_posicao(float x_atual, float y_atual, float z_atual);
void executar_ciclo(void);

#endif // CONTROLE_POSICOES_H