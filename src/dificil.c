#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include "timer.h"
#include "dificil.h"
#include "keyboard.h"
#include "screen.h"
#include "menu.h"

#include <string.h>
#include "leaderboard.h"


// Inicializa variáveis globais
Hunter hunters3[MAX_HUNTERS_DIFICIL];
Door doors[MAX_DOORS];
struct jogador jogador;
struct jogador *head = NULL;


void initializeDoors() {
    int doorCount = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '.' && doorCount < MAX_DOORS) {
                doors[doorCount].x = x;
                doors[doorCount].y = y;
                doors[doorCount].locked = 0;
                map[y][x] = 'l'; // Inicializa como 'l' para indicar porta destrancada
                doorCount++;
            }
        }
    }
}

// Função para posicionar um item em uma posição aleatória no mapa
void placeRandomItem3(int *x, int *y) {
    do {
        *x = rand() % WIDTH;
        *y = rand() % HEIGHT;
    } while (map[*y][*x] == '#' || (playerX == *x && playerY == *y));
}

// Função de tela de vitória
void victoryScreen3(char *nome) {
  // Declara jogador localmente
    jogador.tempo = getTimeDiff();

    screenDestroy();
    screenClear();
    printf("\033[2J\033[H");
    
    printf("###########################\n");
    printf("#         VITÓRIA         #\n");
    printf("###########################\n");
    printf("Pressione uma tecla para encerrar o jogo!\n");
    printf("\033[0m");
    printf("\033[?25h");

   

    loadwinnerlist(&head);
    add_jogador(&head, jogador.nome, jogador.tempo);
    printwinnerlist(head);
    writewinnerlist(head);
    free_winnerlist(head);

    keyboardDestroy();

    exit(0);
              

   
}

// Função de tela de Game Over
void gameOverScreen3() {
    gameOver = 1;
    screenDestroy();
    screenClear();
    printf("\033[2J\033[H");
    printf("###########################\n");
    printf("#        GAME OVER        #\n");
    printf("###########################\n");
    printf("Pressione uma tecla para encerrar o jogo!\n");
    printf("\033[0m");
    printf("\033[?25h");

    keyboardDestroy();

    exit(0);
 
   
   
}

// Função para mover todos os caçadores em direção ao jogador
void moveHunters3() {
    for (int i = 0; i < MAX_HUNTERS_DIFICIL; i++) {
        if (hunters3[i].active) {
            int dx = 0, dy = 0;

            // Determina direção para o jogador
            if (hunters3[i].x < playerX) dx = 1;
            else if (hunters3[i].x > playerX) dx = -1;

            if (hunters3[i].y < playerY) dy = 1;
            else if (hunters3[i].y > playerY) dy = -1;

            int targetX = hunters3[i].x + dx;
            int targetY = hunters3[i].y + dy;

            // Verifica se a direção pretendida contém uma porta trancada
            if (map[targetY][targetX] == '|') {
                // Jason quebra a porta e não se move nesta iteração
                map[targetY][targetX] = ' '; // Quebra a porta
                printf("Jason quebrou uma porta trancada em (%d, %d)!\n", targetX, targetY);
                continue; // Para a iteração atual sem mover o caçador
            }

            // Verifica se pode mover para a posição pretendida
            if (canMove3(targetX, targetY)) {
                hunters3[i].x = targetX;
                hunters3[i].y = targetY;
            } else {
                // Tenta contornar obstáculos
                if (canMove3(hunters3[i].x, hunters3[i].y + dy)) { // Move só no eixo Y
                    hunters3[i].y += dy;
                } else if (canMove3(hunters3[i].x + dx, hunters3[i].y)) { // Move só no eixo X
                    hunters3[i].x += dx;
                } else if (canMove3(hunters3[i].x - dx, hunters3[i].y)) { // Tenta oposto em X
                    hunters3[i].x -= dx;
                } else if (canMove3(hunters3[i].x, hunters3[i].y - dy)) { // Tenta oposto em Y
                    hunters3[i].y -= dy;
                }
            }

            // Verifica colisão com o jogador
            if (hunters3[i].x == playerX && hunters3[i].y == playerY) {
                gameOverScreen3();
            }
        }
    }
}


// Função para verificar se o jogador pode se mover para uma posição específica
int canMove3(int newX, int newY) {
    return map[newY][newX] != '#' && map[newY][newX] != '|';
}

// Função para inicializar o jogo, incluindo os itens e reset dos caçadores
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

// Função para adicionar novos caçadores a cada 5 segundos em uma posição aleatória
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

// Função para adicionar portas aleatoriamente no mapa


// Função para verificar se o jogador está próximo de uma porta destrancada
int isAdjacentToDoor(int x, int y) {
    for (int i = 0; i < MAX_DOORS; i++) {
        if (doors[i].x != -1) { // Detecta qualquer porta, aberta ou fechada
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


// Função para trancar uma porta específica
void lockDoor(int doorIndex) {
    if (doors[doorIndex].locked == 0 && map[doors[doorIndex].y][doors[doorIndex].x] == 'l') {
        doors[doorIndex].locked = 1;
        map[doors[doorIndex].y][doors[doorIndex].x] = '|';  // Tranca a porta, substituindo 'l' por '|'
        printf("Porta trancada!\n");
    }
}
// Função que renderiza o mapa e todos os elementos (jogador, caçadores, itens)
void renderMapWithHUD3() {
    printf("\033[2J\033[H");
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int termWidth = w.ws_col;
    int termHeight = w.ws_row;
    int marginX = (termWidth - WIDTH) / 2;
    int marginY = (termHeight - HEIGHT - 3) / 2;

    for (int y = 0; y < marginY; y++) {
        printf("\n");
    }

    // HUD
    for (int i = 0; i < marginX; i++) printf(" ");
    printf("🔑 %s | ⛽: %s\n", hasKey ? "Coletada" : "Não coletada", hasGasoline ? "Coletada" : "Não coletada");

    for (int y = 0; y < HEIGHT; y++) {
        for (int i = 0; i < marginX; i++) printf(" ");
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("P"); // Representação do jogador
            } else {
                int isHunter = 0;
                for (int i = 0; i < MAX_HUNTERS_DIFICIL; i++) {
                    if (hunters3[i].active && x == hunters3[i].x && y == hunters3[i].y) {
                        printf("J"); // Representação do caçador
                        isHunter = 1;
                        break;
                    }
                }
                if (!isHunter) {
                    if (x == keyX && y == keyY && !hasKey) {
                        printf("🔑"); // Representação da chave
                    } else if (x == gasX && y == gasY && !hasGasoline) {
                        printf("⛽"); // Representação da gasolina
                    } else if (x == carX && y == carY) {
                        printf("C"); // Representação do carro
                    } else if (map[y][x] == '#') {
                        printf("█"); // Representação da parede
                    } else if (map[y][x] == '|') {
                        printf("|"); // Representação da porta trancada
                    } else if (map[y][x] == 'l') {
                        printf("/"); // Porta destrancada
                    } else {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    
}

// Função para verificar se o jogador coletou algum item
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
        victoryScreen3(jogador.nome);
    }
}

// Função para movimentar o jogador
void movePlayer3(char direction) {
    int newX = playerX;
    int newY = playerY;

    switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        case 'r': {
            int doorIndex = isAdjacentToDoor(playerX, playerY);
            if (doorIndex != -1) {
                lockDoor(doorIndex);
            }
            return; // Interrompe, pois "r" não é um movimento
        }
    }

    if (canMove3(newX, newY)) {
        playerX = newX;
        playerY = newY;
        checkForItems3();
    } else {
        printf("\nColisão detectada! Não é possível atravessar a parede.\n");
    }
}


void telaverificar(char *nome, int tamanho) {
    keyboardDestroy();
    printf("Digite seu nome: ");
    
    fgets(nome, tamanho, stdin);
    nome[strcspn(nome, "\n")] = 0;
}
// Função para movimentar os caçadores
void* hunterMovement3(void* arg) {
    while (!gameOver) {
        moveHunters3();
        renderMapWithHUD3();
        sleep(1);
    }
    return NULL;
}

// Função para criar portas periodicamente

// Função principal do jogo
void startGameDificil() {
    gameOver = 0;
   

    // Captura o nome do jogador
    telaverificar(jogador.nome, sizeof(jogador.nome));

    initializeGame3();
    keyboardInit();
    timerInit(0);

    pthread_t hunterThread, spawnThread;
    pthread_create(&hunterThread, NULL, hunterMovement3, NULL);
    pthread_create(&spawnThread, NULL, spawnHunter3, NULL);

    char input;
    while (1) {
        renderMapWithHUD3();
        scanf("%c", &input);
        movePlayer3(input);
        if (gameOver) {
            break;
        }
    }

    pthread_join(hunterThread, NULL);
    pthread_join(spawnThread, NULL);

    keyboardDestroy();
    // #
}