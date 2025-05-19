#ifndef NEXTION_INTERFACE_H
#define NEXTION_INTERFACE_H

#include "mbed.h"
#include <cstring>
#include <vector>

// Inicializa UART e configura interrupção do Nextion
void iniciar_nextion();

// Atualiza comando recebido via UART
bool atualizar_comando();

// Funções booleanas para botões (agora recebem referência a variável para ativação)
void botao_referenciamento(bool &variavel);
void botao_mais(bool &variavel);
void botao_menos(bool &variavel);
void botao_iniciar_sistema(bool &variavel);
void botao_salvar_posicao(bool &variavel);
void botao_ok(bool &variavel);


// Funções para botão de movimentação do eixo Z (retornam true/false direto)
bool botao_z_cima(bool &variavel);   // true se comando == "ZUT"
bool botao_z_baixo(bool &variavel);  // true se comando == "ZDT"

// Funções de comunicação com o Nextion
void enviar_texto_nextion(const char* componente, const char* texto);
void atualizar_status();
void atualizar_t0(const char* texto);
void atualizar_t1(const char* texto);
void atualizar_t2(const char* texto);


#endif // NEXTION_INTERFACE_H
