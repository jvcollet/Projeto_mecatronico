// // controle_posicoes.cpp

// #include "mbed.h"
// #include "controle_posicoes.h"
// #include "movimento.h"      // Para step_forward, step_backward
// #include "referenciamento.h" // Para acesso à posição global

// extern BusOut MP1;
// extern BusOut MP2;
// extern BusOut MP3;

// extern int x_posicao, y_posicao, z_posicao;

// // Lista de posições salvas
// Posicao posicoes[MAX_POSICOES];
// int total_posicoes = 0;

// // Função para salvar uma nova posição
// void salvar_posicao(float x_atual, float y_atual, float z_atual) {
//     if (total_posicoes < MAX_POSICOES) {
//         posicoes[total_posicoes].x = x_atual;
//         posicoes[total_posicoes].y = y_atual;
//         posicoes[total_posicoes].z = z_atual;
//         printf("\nPosição %d salva: (%.2f, %.2f, %.2f)\n", total_posicoes, x_atual, y_atual, z_atual);
//         total_posicoes++;
//     } else {
//         printf("\nLimite máximo de posições atingido!\n");
//     }
// }

// // Função para mover para uma posição específica
// void mover_para_posicao(float alvo_x, float alvo_y) {
//     while (x_posicao < alvo_x - 1) { step_forward(MP1, x_posicao); }
//     while (x_posicao > alvo_x + 1) { step_backward(MP1, x_posicao); }
//     while (y_posicao < alvo_y - 1) { step_forward(MP2, y_posicao); }
//     while (y_posicao > alvo_y + 1) { step_backward(MP2, y_posicao); }
//     // Eixo Z você pode melhorar depois: para descer/coletar e subir
// }

// // Executa ciclo completo: passa por todas as posições salvas
// void executar_ciclo() {
//     printf("\nIniciando ciclo automático...\n");
//     for (int i = 0; i < total_posicoes; i++) {
//         printf("\nMovendo para posição %d\n", i);
//         mover_para_posicao(posicoes[i].x, posicoes[i].y);
//         wait(1.5);  // Tempo simulado de coleta/dispensa
//     }
//     printf("\nCiclo automático finalizado.\n");
// }
