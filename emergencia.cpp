// === emergencia.cpp ===
#include "emergencia.h"
#include "nextion_interface.h"

// Declarar o pino de emergência e o buzzer, se já estiverem no main.cpp
extern DigitalIn emergencia;
extern DigitalOut ENABLE_X;
extern DigitalOut ENABLE_Y;
extern BusOut MP3;
extern DigitalOut buzzer;

void checar_emergencia_com_reset(void) {
    if (emergencia.read() == 0) {
        atualizar_t0("!!! EMERGÊNCIA !!!");
        atualizar_t1("Sistema parado - reiniciando...");
        ENABLE_X = 1;
        ENABLE_Y = 1;
        MP3 = 0;
        buzzer = 1;
        wait_ms(3000); // tempo para o operador ver o alerta
        NVIC_SystemReset();
    }
}
