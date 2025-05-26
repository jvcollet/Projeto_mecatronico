#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"
#include "nextion_interface.h"
#include "emergencia.h"


// --- Saídas dos drivers X, Y e Z ---
extern DigitalOut DIR_X;
extern DigitalOut CLK_X;
extern DigitalOut ENABLE_X;
extern DigitalOut DIR_Y;
extern DigitalOut CLK_Y;
extern DigitalOut ENABLE_Y;

// Usamos MP3 como saída para Z (IN1..IN4)
extern BusOut    MP3;

// --- Fim de curso X, Y e Z ---
extern DigitalIn xMin;
extern DigitalIn xMax;
extern DigitalIn yMin;
extern DigitalIn yMax;
extern DigitalIn zMin;
extern DigitalIn zMax;

// --- Posições atuais ---
extern int x_posicao;
extern int y_posicao;
extern int z_posicao;

#define VELO_HOMING    200   // meio período de pulso (us)
#define VELO_HOMING_Z    3000   // meio período de pulso (us)
int x_posicao_max = -42500;
int y_posicao_max = 41000;
int z_posicao_max = -1225; // ajuste conforme necessidade
static const uint8_t STEP_PATTERN[4] = {1<<0, 1<<1, 1<<2, 1<<3};
 

// --- Função de pulso para X e Y ---
static void pulso_step(DigitalOut &CLK) {
    CLK = 1;
    wait_us(VELO_HOMING);
    CLK = 0;
    wait_us(VELO_HOMING);
}

// --- Funções de passo X, Y e Z ---
void step_x(int direction, int &pos) {
    ENABLE_X = 0;
    DIR_X    = (direction < 0) ? 1 : 0;
    pulso_step(CLK_X);
    pos += (direction < 0) ? -1 : +1;
}

void step_y(int direction, int &pos) {
    ENABLE_Y = 0;
    DIR_Y    = (direction > 0) ? 1 : 0;
    pulso_step(CLK_Y);
    pos += (direction < 0) ? -1 : +1;
}

void step_z(int direction, int &pos) {
    if (direction > 0) {
        for (int i = 3; i >= 0; --i) {
            MP3 = STEP_PATTERN[i];
            wait_us(VELO_HOMING_Z);
        }
        pos--;
    } else {
        for (int i = 0; i < 4; ++i) {
            MP3 = STEP_PATTERN[i];
            wait_us(VELO_HOMING_Z);
        }
        pos++;
    }
    MP3 = 0;
}

// --- Controle contínuo via joystick para X e Y ---
Ticker tickerX, tickerY;
volatile bool joystickMoveX = false;
volatile bool joystickMoveY = false;
volatile int joystickDirX = 0;
volatile int joystickDirY = 0;
bool joystickTickerStarted = false;

void pulso_joystick_X() {
    if (joystickMoveX) {
        ENABLE_X = 0;
        DIR_X = (joystickDirX == 1) ? 0 : 1;
        CLK_X = !CLK_X;
        if (CLK_X) x_posicao += (joystickDirX == 1) ? +1 : -1;
    } else {
        CLK_X = 0;
    }
}

void pulso_joystick_Y() {
    if (joystickMoveY) {
        ENABLE_Y = 0;
        DIR_Y = (joystickDirY == 1) ? 0 : 1;
        CLK_Y = !CLK_Y;
        if (CLK_Y) y_posicao += (joystickDirY == 1) ? -1 : +1;
    } else {
        CLK_Y = 0;
    }
}

// --- Detecção de botões Nextion para Z ---
static volatile bool zNextionUp = false;
static volatile bool zNextionDown = false;

// --- Movimento manual: X/Y via joystick e Z via Nextion ---
void movimento_manual(int x_joystick, int y_joystick, bool manual) {
    if (!manual) return;

    // X via joystick
    if (x_joystick < 400 && x_posicao < 0 && xMax.read() == 1) {
        step_x(+1, x_posicao);  // mover para esquerda
        wait_us(10);
    } else if (x_joystick > 600 && x_posicao > x_posicao_max && xMin.read() == 1) {
        step_x(-1, x_posicao);  // mover para direita
        wait_us(10);
    }

    // Y via joystick
    if (y_joystick > 600 && y_posicao < y_posicao_max && yMin.read() == 1) {
        step_y(-1, y_posicao);  // mover para frente
        wait_us(10);
    } else if (y_joystick < 400 && y_posicao < 0 && yMax.read() == 1) {
        step_y(+1, y_posicao);  // mover para trás
        wait_us(10);
    }

    // Z via Nextion
    bool zUp = false, zDown = false;
    botao_z_cima(zUp);   // zUp = true ao receber “ZUT”
    botao_z_baixo(zDown); // zDown = true ao receber “ZDT”

    // SUBIR
    if (zDown && zMax.read() == 1 && z_posicao > z_posicao_max) {
        step_z(+1, z_posicao);
    }
    // DESCER
    else if (zUp && zMin.read() == 1 && z_posicao <= 0) {
        step_z(-1, z_posicao);
    }
}