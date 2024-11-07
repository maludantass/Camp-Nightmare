#ifndef MAP_H
#define MAP_H

#define WIDTH 60
#define HEIGHT 30

// Declaração das variáveis globais
extern char map[HEIGHT][WIDTH];
extern int playerX;
extern int playerY;
extern int hunterX;
extern int hunterY;
extern int gameOver;  // Variável de controle para Game Over

// Declaração das funções
void renderMap();
int canMove(int newX, int newY);
void movePlayer(char direction);
void moveHunter();
void startGame();
void gameOverScreen(); // Declaração da função de Game Over

#endif // MAP_H
