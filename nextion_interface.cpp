#include "mbed.h"
#include <cstring>
#include <vector>
#include <stdbool.h>
#include <string.h>
#include "nextion_interface.h"
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

void enviar_texto_nextion(const char* componente, const char* texto) {
    static char ultimo_componente[20] = "";
    static char ultimo_texto[64] = "";

    if (strcmp(componente, ultimo_componente) != 0 || strcmp(texto, ultimo_texto) != 0) {
        nextion.printf("%s.txt=\"%s\"", componente, texto);
        nextion.putc(0xFF); nextion.putc(0xFF); nextion.putc(0xFF);
        strncpy(ultimo_componente, componente, sizeof(ultimo_componente) - 1);
        strncpy(ultimo_texto, texto, sizeof(ultimo_texto) - 1);
        wait_ms(400); // reduz risco de 429
    }
}

// Função utilitária: compara se duas strings têm o mesmo multiconjunto de caracteres
bool same_letters(const char* a, const char* b) {
    size_t la = strlen(a), lb = strlen(b);
    if (la != lb) return false;

    int counts[256] = {0};
    for (size_t i = 0; i < la; i++) {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];
        counts[ca]++;
        counts[cb]--;
    }
    for (int i = 0; i < 256; i++) {
        if (counts[i] != 0) return false;
    }
    return true;
}

// Macro pra ficar mais legível
#define IS_CMD(cmd, exp)  (same_letters(cmd, exp))

void rx_handler() {
    static int count = 0;
    while (nextion.readable()) {
        char c = nextion.getc();
        // Ignora \n, \r e 0xFF
        if (c == '\n' || c == '\r' || c == 0xFF || c == 0x20) continue;
        if (count < 3) {
            comando_atual[count++] = c;
        }

        if (count == 3) {
            comando_atual[3] = '\0';      // Finaliza string corretamente
            comando_disponivel = true;
            count = 0;
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
        //atualizar_t0(comando_atual);
        return true;
    }
    return false;
}

// Botões  interface Nextion
void botao_iniciar_sistema(bool &variavel) {
    if (IS_CMD(comando_atual, "I_S")) {
        variavel = true;
        strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
    }
}

void botao_salvar_posicao(bool &variavel) {
    if (IS_CMD(comando_atual, "S_P")) {
        variavel = true;
        strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
    }
}

void botao_ok(bool &variavel) {
    if (IS_CMD(comando_atual, "_OK")) {
        variavel = true;
        strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
    }
}

void botao_referenciamento(bool &variavel) {
    if (IS_CMD(comando_atual, "REF")) {
        variavel = true;
        strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
    }
}

void botao_mais(bool &variavel) {
    if (IS_CMD(comando_atual, "+ml")) {
        variavel = true;
        strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
    }
}

void botao_menos(bool &variavel) {
    if (IS_CMD(comando_atual, "-ml")) {
        variavel = true;
        strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
    }
}

bool botao_z_cima() {
    return IS_CMD(comando_atual, "ZUT");
    strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
}

bool botao_z_baixo() {
    return IS_CMD(comando_atual, "ZDT");
    strcpy(comando_atual, "XXX"); // LIMPA O COMANDO AQUI
}


// Comandos de mudança de texto
void atualizar_status() {
    enviar_texto_nextion("t0", status1);
    enviar_texto_nextion("t1", status2);
    enviar_texto_nextion("t2", status2);
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
    static char status3[64];
    strncpy(status3, texto, sizeof(status3) - 1);
    status3[sizeof(status3) - 1] = '\0';
    enviar_texto_nextion("t2", status3);
}
