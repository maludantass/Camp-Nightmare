#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include "medio.h"
#include "screen.h"

Hunter hunters[MAX_HUNTERS]; // Array de caçadores, inicializado aqui

// Função para verificar se o jogador pode se mover para uma posição específica
int canMove2(int newX, int newY) {
    return map[newY][newX] != '#';
}

// Função para posicionar um item em uma posição aleatória no mapa
void placeRandomItem2(int *x, int *y) {
    do {
        *x = rand() % WIDTH;
        *y = rand() % HEIGHT;
    } while (map[*y][*x] == '#' || (playerX == *x && playerY == *y));
}

// Função para inicializar o jogo, incluindo os itens e reset dos caçadores
void initializeGame2() {
    srand(time(NULL));
    placeRandomItem2(&keyX, &keyY);
    placeRandomItem2(&gasX, &gasY);
    placeRandomItem2(&carX, &carY);
    for (int i = 0; i < MAX_HUNTERS; i++) {
        hunters[i].active = 0; // Inicialmente, todos os caçadores estão inativos
    }
}

// Função que adiciona novos caçadores a cada 5 segundos em uma posição aleatória
void* spawnHunter(void* arg) {
    while (!gameOver) {
        sleep(5);
        for (int i = 0; i < MAX_HUNTERS; i++) {
            if (!hunters[i].active) {
                placeRandomItem2(&hunters[i].x, &hunters[i].y);
                hunters[i].active = 1; // Ativa o novo caçador
                break;
            }
        }
    }
    return NULL;
}

// Função para renderizar o HUD do inventário

void renderInventoryHUD(int marginX) {
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

// Função para renderizar o mapa e todos os elementos (jogador, caçadores, itens) junto com o HUD
void renderMapWithHUD() {
    screenClear(); // Limpa a tela e move o cursor para o canto superior esquerdo

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int termWidth = w.ws_col;    // Largura atual do terminal
    int termHeight = w.ws_row;   // Altura atual do terminal

    int marginX = (termWidth - WIDTH) / 2;
    int marginY = (termHeight - HEIGHT - 3) / 2; // Espaço para HUD e mapa

    // Adiciona linhas em branco no topo para centralizar verticalmente
    for (int i = 0; i < marginY; i++) {
        printf("\n");
    }

    // Renderiza o HUD com o separador centralizado usando marginX
    renderInventoryHUD(marginX);

    // Renderiza o mapa, centralizado horizontalmente
    for (int y = 0; y < HEIGHT; y++) {
        for (int i = 0; i < marginX; i++) {
            printf(" ");
        }
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                screenSetColor(WHITE, BLACK); // Define a cor do jogador como branca
                printf("P");
            } else {
                int isHunter = 0;
                for (int i = 0; i < MAX_HUNTERS; i++) {
                    if (hunters[i].active && x == hunters[i].x && y == hunters[i].y) {
                        screenSetColor(LIGHTRED, BLACK); // Define a cor do caçador como vermelho claro
                        printf("H");
                        isHunter = 1;
                        break;
                    }
                }
                if (!isHunter) {
                    if (x == keyX && y == keyY && !hasKey) {
                        screenSetColor(YELLOW, BLACK); // Define a cor da chave como amarela
                        printf("K");
                    } else if (x == gasX && y == gasY && !hasGasoline) {
                        screenSetColor(GREEN, BLACK); // Define a cor da gasolina como verde
                        printf("G");
                    } else if (x == carX && y == carY) {
                        screenSetColor(CYAN, BLACK); // Define a cor do carro como ciano
                        printf("C");
                    } else if (map[y][x] == '#') {
                        screenSetColor(RED, BLACK); // Define a cor das paredes como vermelha
                        printf("█");
                    } else {
                        screenSetColor(WHITE, BLACK); // Define a cor de fundo para áreas vazias
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    screenSetColor(WHITE, BLACK); // Restaura a cor padrão ao final
    fflush(stdout); // Garante que o conteúdo seja impresso no terminal
}

// Função que verifica se o jogador coletou algum item
void checkForItems2() {
    if (playerX == keyX && playerY == keyY && !hasKey) {
        hasKey = 1;
        printf("Você pegou a chave!\n");
    }
    if (playerX == gasX && playerY == gasY && !hasGasoline) {
        hasGasoline = 1;
        printf("Você pegou a gasolina!\n");
    }
    if (playerX == carX && playerY == carY && hasKey && hasGasoline) {
        victoryScreen2();
    }
}

// Função para mover todos os caçadores em direção ao jogador
void moveHunters2() {
    for (int i = 0; i < MAX_HUNTERS; i++) {
        if (hunters[i].active) {
            int dx = 0, dy = 0;

            if (hunters[i].x < playerX) dx = 1;
            else if (hunters[i].x > playerX) dx = -1;

            if (hunters[i].y < playerY) dy = 1;
            else if (hunters[i].y > playerY) dy = -1;

            if (canMove2(hunters[i].x + dx, hunters[i].y)) hunters[i].x += dx;
            if (canMove2(hunters[i].x, hunters[i].y + dy)) hunters[i].y += dy;

            if (hunters[i].x == playerX && hunters[i].y == playerY) {
                gameOverScreen2();
            }
        }
    }
}

// Função para mover o jogador
void movePlayer2(char direction) {
    int newX = playerX;
    int newY = playerY;

    switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
    }

    if (canMove2(newX, newY)) {
        playerX = newX;
        playerY = newY;
        checkForItems2(); // Verifica se o jogador coletou algum item
    } else {
        printf("\nColisão detectada! Não é possível atravessar a parede.\n");
    }
}

// Função de tela de vitória
void victoryScreen2() {
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
void gameOverScreen2() {
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

// Função da thread para movimentar os caçadores
void* hunterMovement2(void* arg) {
    while (!gameOver) {
        moveHunters2();
        renderMapWithHUD();
        sleep(1);
    }
    return NULL;
}

// Função principal do jogo no modo médio
void startGameMedio() {
    initializeGame2();

    pthread_t hunterThread, spawnThread;
    if (pthread_create(&hunterThread, NULL, hunterMovement2, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread de movimentação dos caçadores.\n");
        exit(1);
    }
    if (pthread_create(&spawnThread, NULL, spawnHunter, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread de spawn dos caçadores.\n");
        exit(1);
    }

    char input;
    while (!gameOver) {
        renderMapWithHUD();
        scanf(" %c", &input);
        movePlayer2(input);
        usleep(50000);
    }

    pthread_join(hunterThread, NULL);
    pthread_join(spawnThread, NULL);
}



