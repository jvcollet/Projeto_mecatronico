// === main.cpp ===
#include "mbed.h"
#include "movimento.h"
#include "referenciamento.h"
#include "controle_posicoes.h"
#include "nextion_interface.h"
#include "pipetadora.h"


// Drivers de passo para X e Y
DigitalOut DIR_X(PB_10);
DigitalOut CLK_X(PA_8);
DigitalOut ENABLE_X(PB_4, 0);  // enable ativo baixo
DigitalOut DIR_Y(PB_3);
DigitalOut CLK_Y(PB_5);
DigitalOut ENABLE_Y(PA_10, 0);  // enable ativo baixo

// Motor Z via BusOut
BusOut MP3(D13, D14, D15, D15);

// Sensores de fim de curso
DigitalIn yMax(PB_9);  // Y 
DigitalIn yMin(PB_8);  // Y 
DigitalIn xMin(PA_11); // X
DigitalIn xMax(PA_12); // X
DigitalIn zMin(D0);
DigitalIn zMax(D1);

// Contadores de posicao
int x_posicao = 0;
int y_posicao = 0;
int z_posicao = 0;

// Controles adicionais
DigitalOut Led(LED1);
DigitalIn  botao(PC_13);
AnalogIn   xAxis(PA_1);
AnalogIn   yAxis(PA_0);

// Botao de emergencia
DigitalIn emergencia(PC_7);

//Acionamento pipeta
DigitalIn mybutton(USER_BUTTON);
DigitalOut pipetadora(D13); // Rele

// Comunicacao com Nextion (declarado em nextion_interface.cpp)
extern Serial nextion;
extern char comando_atual[10];
extern bool pronto_iniciar;

extern int estado_interface;
extern int total_dispensas;
extern int volume_atual;
extern int posicao_index;
extern int total_posicoes;
extern bool coleta_salva;



int main() {
    
    iniciar_nextion();
    inicializar_pipetadora();
    
    // Verifica emergência (botão ativo baixo)
        if (emergencia.read() == 0) {
            // Para todo movimento e exibe alerta
            atualizar_t0("!!! EMERGENCIA !!!");
            atualizar_t1("Sistema parado");
            ENABLE_X = 1;
            ENABLE_Y = 1;
            while (true) {
                // Piscar LED para sinalizar emergência
                Led = !Led;
                wait_ms(500);
            }
        }
    // Estados de botões Nextion
    bool btn_iniciar = false;
    bool btn_ok = false;
    bool btn_mais = false;
    bool btn_menos = false;
    bool referenciar_sistema = false;

    // Mensagens iniciais
    atualizar_t0("Clique no botao de referenciar");
    atualizar_t1("Sistema iniciado");
    atualizar_t2("Posicoes ainda nao refernciadas");

    bool modo_manual = true; 

    while (true) {
       
        botao_referenciamento(referenciar_sistema);

        int xv = xAxis.read() * 1000;
        int yv = yAxis.read() * 1000;

        movimento_manual(xv, yv, modo_manual);
 
        
        if (referenciar_sistema) {
            atualizar_t0("Aguarde o refernciamento");
            atualizar_t1("Referenciando...");
            referenciar();            

            atualizar_t0("Clique iniciar o sistema");
            atualizar_t1("Aguardando comando");
        
            referenciar_sistema = false;
            btn_iniciar = false;
            btn_ok = false;
            btn_mais = false;
            btn_menos = false;
            estado_interface = 0;
            total_dispensas = 1;
            volume_atual = 1;
            posicao_index = 0;
            coleta_salva = false;
            pronto_iniciar = false;
        }
        
        // Formato para colocar a posição do X e Y
        char buffer[64];
        sprintf(buffer, "Posicao- X: %d Y: %d", x_posicao, y_posicao);
        atualizar_t2(buffer);
        
        // Lógica da interface (controle_posicoes.cpp)
        botao_iniciar_sistema(btn_iniciar);
        if (btn_iniciar){
            
            botao_mais(btn_mais);
            botao_menos(btn_menos);
            botao_iniciar_sistema(btn_iniciar);
            botao_ok(btn_ok);
        
            logica_interface_usuario(btn_iniciar, btn_mais, btn_menos, btn_ok);
            wait_ms(100);
            btn_ok = false;
            btn_mais = false;
            btn_menos = false;
            referenciar_sistema = false;
            if (pronto_iniciar){
                executar_ciclo();
            }
        }
    }
}