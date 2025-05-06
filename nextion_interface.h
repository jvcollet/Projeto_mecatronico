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
void botao_salvar_coleta(bool &variavel);
void botao_salvar_dispensa(bool &variavel);
void botao_referenciamento(bool &variavel);
void botao_iniciar_ciclo(bool &variavel);
void botao_aumentar_volume(bool &variavel);
void botao_diminuir_volume(bool &variavel);

// Funções para botão de movimentação do eixo Z (retornam true/false direto)
bool botao_z_cima();   // true se comando == "ZUT"
bool botao_z_baixo();  // true se comando == "ZDT"

// Funções de comunicação com o Nextion
void enviar_texto_nextion(const char* componente, const char* texto);
void atualizar_status();
void atualizar_t0(const char* texto);
void atualizar_t1(const char* texto);
void atualizar_t2(const char* texto);


#endif // NEXTION_INTERFACE_H
