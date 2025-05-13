// pipetadora.cpp
#include "mbed.h"
#include "pipetadora.h"

DigitalOut rele_pipeta(PA_6); // Pino que aciona a pipeta (rele)

void inicializar_pipetadora() {
    rele_pipeta = 0; // Desliga pipeta inicialmente
}

void acionar_coleta() {
    rele_pipeta = 1;
    wait_ms(1000); // Tempo de sucção
    rele_pipeta = 0;
}

void acionar_dispensa() {
    rele_pipeta = 1;
    wait_ms(1000); // Tempo de descarga
    rele_pipeta = 0;
}
