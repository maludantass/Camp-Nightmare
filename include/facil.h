#ifndef FACIL_H
#define FACIL_H

#include "hunter.h"

#define WIDTH 60
#define HEIGHT 20
#define MAX_HUNTERS_FACIL 3

// Declaração das variáveis globais e funções específicas do modo fácil...





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
void renderMap1();
int canMove1(int newX, int newY);
void movePlayer1(char direction);
void moveHunters1();            // Função que move todos os caçadores ativos
void startGameFacil();
void gameOverScreen1();         // Declaração da função de Game Over
void victoryScreen1();          // Declaração da função de Tela de Vitória
void initializeGame1();         // Inicializa o jogo com a posição dos itens
void checkForItems1();          // Verifica se o jogador coletou um item
void placeRandomItem1(int *x, int *y); // Posiciona um item aleatoriamente no mapa
void* spawnHunter1(void* arg);         // Thread que adiciona caçadores a cada 5 segundos
void* hunterMovement1(void* arg);      // Thread que controla o movimento dos caçadores

#endif // FACIL_H
