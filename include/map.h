// include/map.h
#ifndef MAP_H
#define MAP_H

#define WIDTH 60
#define HEIGHT 30

// Declaração das variáveis globais
extern char map[HEIGHT][WIDTH];
extern int playerX;
extern int playerY;
extern int hunterX; // Adicione esta linha para declarar hunterX
extern int hunterY; // Adicione esta linha para declarar hunterY

// Declaração das funções
void renderMap();
int canMove(int newX, int newY);
void movePlayer(char direction);
void moveHunter(); // Declaração da função de movimentação do caçador
void startGame();

#endif // MAP_H
