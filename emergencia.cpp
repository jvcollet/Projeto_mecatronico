// === emergencia.cpp ===
#include "emergencia.h"
#include "nextion_interface.h"

// Declarar o pino de emergência e o buzzer, se já estiverem no main.cpp
extern DigitalIn emergencia;
extern DigitalOut ENABLE_X;
extern DigitalOut ENABLE_Y;
extern BusOut MP3;
extern DigitalOut buzzer;
bool em_emergencia = false;
bool btn_ok = false;

void checar_emergencia_com_reset(void) {
    if (emergencia.read() == 0) {
        atualizar_t0("!!! EMERGENCIA !!!");
        atualizar_t1("Sistema parado");
        atualizar_t2("Clique em OK para sair");
        ENABLE_X = 1;
        ENABLE_Y = 1;
        MP3 = 0;
        buzzer = 1;
        em_emergencia = true;

        while(em_emergencia){
            botao_ok(btn_ok);
            if (btn_ok){
                NVIC_SystemReset();
            } else {

            }
        }   
    }
}
