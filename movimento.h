// movimento.h
#ifndef MOVIMENTO_H
#define MOVIMENTO_H

#include "mbed.h"

// Função para movimentar motores usando o joystick
void movimento_joystick(int x_joystick, int y_joystick);

// NOVO: Funções para mover motor por passo individualmente
void step_forward(BusOut& motor, int& posicao);
void step_backward(BusOut& motor, int& posicao);

#endif
