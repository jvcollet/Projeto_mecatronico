// pipetadora.cpp
#include "mbed.h"
#include "pipetadora.h"

DigitalOut rele_pipeta(PA_9); // Pino que aciona a pipeta (rele)

void inicializar_pipetadora() {
    rele_pipeta = 1; // Desliga pipeta inicialmente
}

void acionar_coleta() {
    rele_pipeta = 0;
    wait_ms(500); // Tempo de sucção
    rele_pipeta = 1;
}

void acionar_dispensa() {
    rele_pipeta = 0;
    wait_ms(500); // Tempo de sucção
    rele_pipeta = 1;
}
