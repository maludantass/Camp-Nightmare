#ifndef MAP_H
#define MAP_H

#define WIDTH 60
#define HEIGHT 20

// Declaração das variáveis globais
extern char map[HEIGHT][WIDTH];
extern int playerX;
extern int playerY;
extern int hunterX;
extern int hunterY;
extern int gameOver;       // Variável de controle para Game Over
extern int hasKey;         // Variável para verificar se o jogador pegou a chave
extern int hasGasoline;    // Variável para verificar se o jogador pegou a gasolina
extern int keyX, keyY;     // Coordenadas da chave
extern int gasX, gasY;     // Coordenadas da gasolina
extern int carX, carY;     // Coordenadas do carro

// Declaração das funções
void renderMap();
int canMove(int newX, int newY);
void movePlayer(char direction);
void moveHunter();
void startGame();
void gameOverScreen();     // Declaração da função de Game Over
void victoryScreen();      // Declaração da função de Tela de Vitória
void initializeGame();     // Inicializa o jogo com a posição dos itens
void checkForItems();      // Verifica se o jogador coletou um item
void placeRandomItem(int *x, int *y);  // Posiciona um item aleatoriamente no mapa
void* spawnHunter(void* arg);          // Thread que adiciona caçadores a cada 5 segundos
void* hunterMovement(void* arg);    

#endif // MAP_H
