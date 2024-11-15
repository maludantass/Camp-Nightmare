#ifndef MEDIO_H
#define MEDIO_H

#include "hunter.h" // Declaração do array de caçadores em um arquivo separado
#include "screen.h" // Inclui screen.h para configurações de cor


#define WIDTH 60
#define HEIGHT 20
#define MAX_HUNTERS 5 // Número máximo de caçadores no jogo

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
void renderMap2();                  // Renderiza o mapa
void renderInventoryHUD();          // Renderiza o HUD do inventário
int canMove2(int newX, int newY);   // Verifica se o jogador pode se mover para uma posição específica
void movePlayer2(char direction);   // Move o jogador na direção dada
void moveHunters2();                // Move todos os caçadores ativos
void startGameMedio();              // Função principal do jogo no modo médio
void gameOverScreen2();             // Exibe a tela de Game Over
void victoryScreen2();              // Exibe a tela de Vitória
void initializeGame2();             // Inicializa o jogo com a posição dos itens
void checkForItems2();              // Verifica se o jogador coletou um item
void placeRandomItem2(int *x, int *y); // Posiciona um item aleatoriamente no mapa
void* spawnHunter(void* arg);          // Thread que adiciona caçadores a cada 5 segundos
void* hunterMovement2(void* arg);  
void renderMapWithHUD();    // Thread que controla o movimento dos caçadores

#endif // MEDIO_H
