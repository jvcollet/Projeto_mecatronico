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

static int estado_interface = 0;
// Variáveis de posição globais
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

// Lista de posições salvas
static Posicao posicoes[MAX_POSICOES];
static int total_posicoes = 0;

// Posição de coleta única
static Posicao posicao_coleta;
static bool coleta_salva = false;
static bool pronto_iniciar = false;

// Estado da interface

static int total_dispensas = 1;
static int volume_atual = 1;
static int posicao_index = 0;

// Salva a posição atual (em diferentes etapas da interface)
void salvar_posicao(float x_atual, float y_atual, float z_atual);

// Lógica da interface baseada nos botões
void logica_interface_usuario(bool iniciar_sistema, bool mais, bool menos, bool ok);

void executar_ciclo(bool btn_iniciar);

#endif // CONTROLE_POSICOES_H