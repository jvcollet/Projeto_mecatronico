// movimento.cpp
#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"  
#include "nextion_interface.h"

extern DigitalOut DIR_X;
extern DigitalOut CLK_X;
extern DigitalOut ENABLE_X;
extern DigitalOut DIR_Y;
extern DigitalOut CLK_Y;
extern DigitalOut ENABLE_Y;
extern BusOut    MP3;

extern DigitalIn xMin;
extern DigitalIn xMax;
extern DigitalIn yMin;
extern DigitalIn yMax;
extern DigitalIn zMin;
extern DigitalIn zMax;

extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

#define VELO_HOMING    200   // meio período de pulso (s)
int x_posicao_max = 1600;  // passos até o outro fim de curso
int y_posicao_max = 1600;  // passos até o outro fim de curso
static const uint8_t STEP_PATTERN[4] = {1<<0, 1<<1, 1<<2, 1<<3};

bool pode_ir_x_positivo = (x_posicao < x_posicao_max);
bool pode_ir_x_negativo = (x_posicao > 0);
bool pode_ir_y_positivo = (y_posicao < y_posicao_max);
bool pode_ir_y_negativo = (y_posicao > 0);

// Gera pulso de passo para drivers X e Y
static void pulso_step(DigitalOut &CLK) {
    CLK = 1;
    wait_us(VELO_HOMING);
    CLK = 0;
    wait_us(VELO_HOMING);
}

// Funções de passo exportadas
void step_x(int direction, int &pos) {
    ENABLE_X = 0;
    DIR_X    = (direction > 0) ? 1 : 0;
    pulso_step(CLK_X);
    pos += (direction > 0) ? 1 : -1;
}

void step_y(int direction, int &pos) {
    ENABLE_Y = 0;
    DIR_Y    = (direction > 0) ? 1 : 0;
    pulso_step(CLK_Y);
    pos += (direction > 0) ? 1 : -1;
}

void step_z(int direction, int &pos) {
    if (direction > 0) {
        for (int i = 0; i < 4; ++i) {
            MP3 = STEP_PATTERN[i];
            wait_us(VELO_HOMING);
        }
        pos++;
    } else {
        for (int i = 3; i >= 0; --i) {
            MP3 = STEP_PATTERN[i];
            wait_us(VELO_HOMING);
        }
        pos--;
    }
}

// Movimento manual: X/Y via joystick e Z via Nextion, só se manual == true
void movimento_manual(int x_joystick, int y_joystick, bool manual) {
    // Não executa o código se for falso
    if (!manual) return;
    // X e Y pelo joystick
    if (x_joystick > 550 && pode_ir_x_positivo) step_x(+1, x_posicao);
    else if (x_joystick < 450 && pode_ir_x_negativo) step_x(-1, x_posicao);

    if (y_joystick > 550 && pode_ir_y_positivo) step_y(+1, y_posicao);
    else if (y_joystick < 450 && pode_ir_y_negativo) step_y(-1, y_posicao);

    // Z pelos botões Nextion ('U' para subir, 'D' para descer)
    // É preciso adaptar com a parte do nextion depois
    // while (nextion.readable()) {
    //     char c = nextion.getc();
    //     if (c == 'U' && zMax.read()) step_z(+1, z_posicao);
    //     else if (c == 'D' && zMin.read()) step_z(-1, z_posicao);
}
