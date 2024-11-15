#ifndef DIFICIL_H
#define DIFICIL_H

#include "hunter.h"
#include "screen.h"


#define WIDTH 60
#define HEIGHT 20
#define MAX_HUNTERS_DIFICIL 7// Número máximo de caçadores no jogo
#define MAX_DOORS 10  // Número máximo de portas no jogo

// Declaração das variáveis globais
extern char map[HEIGHT][WIDTH];
extern int playerX;
extern int playerY;
extern int gameOver;
extern int hasKey;
extern int hasGasoline;
extern int keyX, keyY;
extern int gasX, gasY;
extern int carX, carY;
extern struct jogador jogador;

// Estrutura para representar uma porta
typedef struct {
    int x;
    int y;
    int locked;
} Door;

extern Door doors[MAX_DOORS]; // Array de portas

// Declaração das funções
void renderMap3();                   // Renderiza o mapa
void renderInventoryHUD3(int marginX); // Renderiza o HUD do inventário com margem horizontal
int canMove3(int newX, int newY);    // Verifica se o jogador pode se mover para uma posição específica
void movePlayer3(char direction);    // Move o jogador na direção especificada
void moveHunters3();                 // Move todos os caçadores ativos
void startGameDificil();             // Função principal do jogo no modo difícil
void gameOverScreen3();              // Exibe a tela de Game Over
void victoryScreen3();               // Exibe a tela de Vitória
void initializeGame3();              // Inicializa o jogo e configura os itens e portas
void checkForItems3();               // Verifica se o jogador coletou um item
void placeRandomItem3(int *x, int *y); // Posiciona um item aleatoriamente no mapa
void* spawnHunter3(void* arg);       // Thread para adicionar caçadores a cada 5 segundos
void* hunterMovement3(void* arg);    // Thread que controla o movimento dos caçadores
void renderMapWithHUD3();            // Renderiza o mapa com o HUD
void* doorSpawner(void* arg);        // Thread para criar portas a cada 3 segundos
int isAdjacentToDoor(int x, int y);  // Verifica se o jogador está adjacente a uma porta destrancada
void lockDoor(int doorIndex);   
void breakDoor(int x, int y);   // Tranca uma porta específica

#endif // DIFICIL_H