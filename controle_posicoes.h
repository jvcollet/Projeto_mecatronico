// controle_posicoes.h
#ifndef CONTROLE_POSICOES_H
#define CONTROLE_POSICOES_H

#include "mbed.h"
#include "movimento.h"  // para protótipos de step_x, step_y, step_z

#define MAX_POSICOES 9

// Estrutura que representa uma posição com volume associado
typedef struct {
    float x;
    float y;
    float z;
    int volume;  // Volume em ml
} Posicao;

// Salva a posição atual (em diferentes etapas da interface)
void salvar_posicao(float x_atual, float y_atual, float z_atual);

// Lógica da interface baseada nos botões
void logica_interface_usuario(bool iniciar_sistema, bool mais, bool menos, bool ok);

void executar_ciclo(void);

#endif // CONTROLE_POSICOES_H