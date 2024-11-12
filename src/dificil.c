#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include "dificil.h"
#include "keyboard.h"

// Inicializa variáveis globais
Hunter hunters3[MAX_HUNTERS_DIFICIL];
Door doors[MAX_DOORS];

// Funções auxiliares de inicialização
void initializeDoors() {
    for (int i = 0; i < MAX_DOORS; i++) {
        doors[i].x = -1;
        doors[i].y = -1;
        doors[i].locked = 0;
    }
}

// Função de tela de vitória
void victoryScreen3() {
    printf("\033[2J\033[H");
    printf("###########################\n");
    printf("#         VITÓRIA         #\n");
    printf("###########################\n");
    printf("Pressione uma tecla para encerrar o jogo!\n");
    printf("\033[0m");
    printf("\033[?25h");
    fflush(stdout);
    exit(0);
}

// Função de tela de Game Over
void gameOverScreen3() {
    printf("\033[2J\033[H");
    printf("###########################\n");
    printf("#        GAME OVER        #\n");
    printf("###########################\n");
    printf("Pressione uma tecla para encerrar o jogo!\n");
    printf("\033[0m");
    printf("\033[?25h");
    fflush(stdout);
    exit(0);
}

// Função para mover todos os caçadores em direção ao jogador
void moveHunters3() {
    for (int i = 0; i < MAX_HUNTERS_DIFICIL; i++) {
        if (hunters3[i].active) {
            int dx = 0, dy = 0;

            if (hunters3[i].x < playerX) dx = 1;
            else if (hunters3[i].x > playerX) dx = -1;

            if (hunters3[i].y < playerY) dy = 1;
            else if (hunters3[i].y > playerY) dy = -1;

            int targetX = hunters3[i].x + dx;
            int targetY = hunters3[i].y + dy;

            if (map[targetY][targetX] == '/') {
                // Jason quebra a porta trancada
                map[targetY][targetX] = ' ';
                printf("Jason quebrou uma porta trancada!\n");
                renderMapWithHUD3();
            } else if (canMove3(targetX, targetY)) {
                hunters3[i].x = targetX;
                hunters3[i].y = targetY;
            }

            if (hunters3[i].x == playerX && hunters3[i].y == playerY) {
                gameOverScreen3();
            }
        }
    }
}


void initializeGame3() {
    srand(time(NULL));
    placeRandomItem3(&keyX, &keyY);
    placeRandomItem3(&gasX, &gasY);
    placeRandomItem3(&carX, &carY);
    initializeDoors();
    for (int i = 0; i < MAX_HUNTERS_DIFICIL; i++) {
        hunters3[i].active = 0;
    }
}

// Funções de movimentação e verificação
int canMove3(int newX, int newY) {
    return map[newY][newX] != '#' && map[newY][newX] != '/';
}

void placeRandomItem3(int *x, int *y) {
    do {
        *x = rand() % WIDTH;
        *y = rand() % HEIGHT;
    } while (map[*y][*x] == '#' || (playerX == *x && playerY == *y));
}

int isAdjacentToDoor(int x, int y) {
    for (int i = 0; i < MAX_DOORS; i++) {
        if (doors[i].x != -1 && !doors[i].locked) {
            if ((doors[i].x == x + 1 && doors[i].y == y) ||
                (doors[i].x == x - 1 && doors[i].y == y) ||
                (doors[i].x == x && doors[i].y == y + 1) ||
                (doors[i].x == x && doors[i].y == y - 1)) {
                return i;
            }
        }
    }
    return -1;
}
void lockDoor(int doorIndex) {
    doors[doorIndex].locked = 1;
    map[doors[doorIndex].y][doors[doorIndex].x] = '/';  // Marca como porta trancada
    printf("Porta trancada!\n");
}

// Função para adicionar uma porta aleatoriamente no mapa
void placeDoor() {
    for (int i = 0; i < MAX_DOORS; i++) {
        if (doors[i].x == -1) {
            int doorX, doorY;
            do {
                doorX = rand() % WIDTH;
                doorY = rand() % HEIGHT;
            } while (map[doorY][doorX] != ' ' || 
                     (doorX == keyX && doorY == keyY) || 
                     (doorX == gasX && doorY == gasY) || 
                     (doorX == carX && doorY == carY));

            doors[i].x = doorX;
            doors[i].y = doorY;
            doors[i].locked = 0;
            map[doorY][doorX] = 'l';
            break;
        }
    }
}

void* doorSpawner(void* arg) {
    while (!gameOver) {
        sleep(3);
        placeDoor();
        renderMapWithHUD3();
    }
    return NULL;
}

// Função principal de movimentação do jogador
void movePlayer3(char direction) {
    int newX = playerX;
    int newY = playerY;

    switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
    }

    if (canMove3(newX, newY)) {
        playerX = newX;
        playerY = newY;
        checkForItems3();
    } else {
        printf("\nColisão detectada! Não é possível atravessar a parede.\n");
    }

    int doorIndex = isAdjacentToDoor(playerX, playerY);
    if (doorIndex != -1 && keyhit() && readch() == 'r') {
        lockDoor(doorIndex);
    }
}

// Função que verifica se o jogador coletou algum item
void checkForItems3() {
    if (playerX == keyX && playerY == keyY && !hasKey) {
        hasKey = 1;
        printf("Você pegou a chave!\n");
    }
    if (playerX == gasX && playerY == gasY && !hasGasoline) {
        hasGasoline = 1;
        printf("Você pegou a gasolina!\n");
    }
    if (playerX == carX && playerY == carY && hasKey && hasGasoline) {
        victoryScreen3();
    }
}




// Funções de threads e renderização
void* spawnHunter3(void* arg) {
    while (!gameOver) {
        sleep(5);
        for (int i = 0; i < MAX_HUNTERS_DIFICIL; i++) {
            if (!hunters3[i].active) {
                placeRandomItem3(&hunters3[i].x, &hunters3[i].y);
                hunters3[i].active = 1;
                break;
            }
        }
    }
    return NULL;
}

void renderInventoryHUD3(int marginX) {
    for (int i = 0; i < marginX; i++) {
        printf(" ");
    }
    printf("Chave: %s | Gasolina: %s\n", hasKey ? "Coletada" : "Não coletada", hasGasoline ? "Coletada" : "Não coletada");

    for (int i = 0; i < marginX; i++) {
        printf(" ");
    }
    for (int i = 0; i < WIDTH; i++) {
        printf("=");
    }
    printf("\n");
}

void renderMapWithHUD3() {
    printf("\033[2J\033[H");
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int termWidth = w.ws_col;
    int termHeight = w.ws_row;
    int marginX = (termWidth - WIDTH) / 2;
    int marginY = (termHeight - HEIGHT - 3) / 2;

    for (int i = 0; i < marginY; i++) {
        printf("\n");
    }

    renderInventoryHUD3(marginX);

    for (int y = 0; y < HEIGHT; y++) {
        for (int i = 0; i < marginX; i++) {
            printf(" ");
        }
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("P");
            } else {
                int isHunter = 0;
                for (int i = 0; i < MAX_HUNTERS_DIFICIL; i++) {
                    if (hunters3[i].active && x == hunters3[i].x && y == hunters3[i].y) {
                        printf("H");
                        isHunter = 1;
                        break;
                    }
                }
                if (!isHunter) {
                    if (x == keyX && y == keyY && !hasKey) {
                        printf("K");
                    } else if (x == gasX && y == gasY && !hasGasoline) {
                        printf("G");
                    } else if (x == carX && y == carY) {
                        printf("C");
                    } else {
                        printf("%c", map[y][x]);
                    }
                }
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void* hunterMovement3(void* arg) {
    while (!gameOver) {
        moveHunters3();
        renderMapWithHUD3();
        sleep(1);
    }
    return NULL;
}

void startGameDificil() {
    initializeGame3();
    keyboardInit();

    pthread_t hunterThread, spawnThread, doorThread;
    pthread_create(&hunterThread, NULL, hunterMovement3, NULL);
    pthread_create(&spawnThread, NULL, spawnHunter3, NULL);
    pthread_create(&doorThread, NULL, doorSpawner, NULL);

    char input;
    while (!gameOver) {
        renderMapWithHUD3();
        scanf(" %c", &input);
        movePlayer3(input);
        usleep(50000);
    }

    pthread_join(hunterThread, NULL);
    pthread_join(spawnThread, NULL);
    pthread_join(doorThread, NULL);
    keyboardDestroy();
}
