#ifndef NEXTION_INTERFACE_H
#define NEXTION_INTERFACE_H

#include "mbed.h"
#include <cstring>
#include <vector>

// Inicializa UART e configura interrupção do Nextion
void iniciar_nextion();

// Atualiza comando recebido via UART
bool atualizar_comando();

// Funções booleanas para botões
bool botao_salvar_coleta();
bool botao_salvar_dispensa();
bool botao_referenciamento();
bool botao_iniciar_ciclo();
bool botao_aumentar_volume();
bool botao_diminuir_volume();

// Funções para botão de movimentação do eixo Z
std::vector<int> botao_z_cima();
std::vector<int> botao_z_baixo();

// Funções de comunicação com o Nextion
void enviar_texto_nextion(const char* componente, const char* texto);
void atualizar_status();
void atualizar_t0(const char* texto);
void atualizar_t1(const char* texto);

#endif // NEXTION_INTERFACE_H
