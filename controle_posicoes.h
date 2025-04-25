// controle_posicoes.h

#ifndef CONTROLE_POSICOES_H
#define CONTROLE_POSICOES_H

#include "mbed.h"

// Número máximo de posições
#define MAX_POSICOES 9

// Estrutura para armazenar uma posição
typedef struct {
    float x;
    float y;
    float z;
} Posicao;

// Funções
void salvar_posicao(float x_atual, float y_atual, float z_atual);
void executar_ciclo();

#endif
