#ifndef MEDIO_H
#define MEDIO_H

#include "hunter.h"//declarei o array em uma arquivo separadon pra ficar mlhr organizado

#define WIDTH 80
#define HEIGHT 30
#define MAX_HUNTERS 5// Número máximo de caçadores no jogo





// Declaração das variáveis globais
extern char map[HEIGHT][WIDTH];
extern int playerX;
extern int playerY;
extern int gameOver;       // Variável de controle para Game Over
extern int hasKey;         // Variável para verificar se o jogador pegou a chave
extern int hasGasoline;    // Variável para verificar se o jogador pegou a gasolina
extern int keyX, keyY;     // Coordenadas da chave
extern int gasX, gasY;     // Coordenadas da gasolina
extern int carX, carY;     // Coordenadas do carro

// Declaração das funções
void renderMap2();
int canMove2(int newX, int newY);
void movePlayer2(char direction);
void moveHunters2();            // Função que move todos os caçadores ativos
void startGameMedio();
void gameOverScreen2();         // Declaração da função de Game Over
void victoryScreen2();          // Declaração da função de Tela de Vitória
void initializeGame2();         // Inicializa o jogo com a posição dos itens
void checkForItems2();          // Verifica se o jogador coletou um item
void placeRandomItem2(int *x, int *y); // Posiciona um item aleatoriamente no mapa
void* spawnHunter(void* arg);          // Thread que adiciona caçadores a cada 5 segundos
void* hunterMovement2(void* arg);      // Thread que controla o movimento dos caçadores

#endif // MEDIO_H
