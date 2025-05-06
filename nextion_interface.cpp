#include "mbed.h"
#include <cstring>
#include <vector>

Serial nextion(PC_10, PC_11);  // TX, RX
Serial pc(USBTX, USBRX);
DigitalOut led1(LED1);

char rx_char;
char rx_buffer[64];
int rx_index = 0;
bool comando_disponivel = false;

char comando_atual[10] = "";

char status1[32] = "Sistema pronto";
char status2[32] = "Aguardando comando";

void enviar_texto_nextion(const char* componente, const char* texto); // <- protótipo
void atualizar_status(); // <- protótipo

void rx_handler() {
    led1 = !led1;
    while (nextion.readable()) {
        rx_char = nextion.getc();
        if (rx_char == '\n' || rx_char == '\r') continue;
        rx_buffer[rx_index++] = rx_char;

        if (rx_index >= 3) {
            rx_buffer[rx_index] = '\0';
            strncpy(comando_atual, rx_buffer, sizeof(comando_atual));
            comando_disponivel = true;
            rx_index = 0;
            break;
        }
    }
}

void iniciar_nextion() {
    nextion.baud(9600);
    pc.baud(9600);
    nextion.attach(&rx_handler, Serial::RxIrq);
    strcpy(comando_atual, "");
    atualizar_status();
}

bool atualizar_comando() {
    if (comando_disponivel) {
        comando_disponivel = false;
        pc.printf("Comando recebido: %s\n", comando_atual);
        return true;
    }
    return false;
}

void botao_salvar_coleta(bool &variavel) {
    if (strcmp(comando_atual, "S_C") == 0) {
        variavel = true;
    }
}

void botao_salvar_dispensa(bool &variavel) {
    if (strcmp(comando_atual, "S_D") == 0) {
        variavel = true;
    }
}

void botao_referenciamento(bool &variavel) {
    if (strcmp(comando_atual, "REF") == 0) {
        variavel = true;
        strcpy(comando_atual, ""); // limpa após usar
    }
}

void botao_iniciar_ciclo(bool &variavel) {
    if (strcmp(comando_atual, "I_C") == 0) {
        variavel = true;
    }
}

void botao_aumentar_volume(bool &variavel) {
    if (strcmp(comando_atual, "+ml") == 0) {
        variavel = true;
    }
}

void botao_diminuir_volume(bool &variavel) {
    if (strcmp(comando_atual, "-ml") == 0) {
        variavel = true;
    }
}

bool botao_z_cima() {
    return (strcmp(comando_atual, "ZUT") == 0); // true para ZUT, false para outros
}

bool botao_z_baixo() {
    return (strcmp(comando_atual, "ZDT") == 0); // true para ZDT, false para outros
}


void enviar_texto_nextion(const char* componente, const char* texto) {
    char cmd[100];
    sprintf(cmd, "%s.txt=\"%s\"", componente, texto);
    nextion.printf("%s", cmd);
    nextion.putc(0xFF);
    nextion.putc(0xFF);
    nextion.putc(0xFF);
}

void atualizar_status() {
    enviar_texto_nextion("t0", status1);
    enviar_texto_nextion("t1", status2);
}

void atualizar_t0(const char* texto) {
    strncpy(status1, texto, sizeof(status1)-1);
    status1[sizeof(status1)-1] = '\0';
    enviar_texto_nextion("t0", status1);
}

void atualizar_t1(const char* texto) {
    strncpy(status2, texto, sizeof(status2)-1);
    status2[sizeof(status2)-1] = '\0';
    enviar_texto_nextion("t1", status2);
}

void atualizar_t2(const char* texto) {
    static char status3[64]; // você pode ajustar o tamanho conforme necessário
    strncpy(status3, texto, sizeof(status3) - 1);
    status3[sizeof(status3) - 1] = '\0';
    enviar_texto_nextion("t2", status3);
}
