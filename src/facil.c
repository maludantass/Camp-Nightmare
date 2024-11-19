#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include "facil.h"
#include "keyboard.h"
#include "screen.h"

Hunter hunters1[MAX_HUNTERS_FACIL];



// Função para verificar se o jogador pode se mover para uma posição específica
int canMove1(int newX, int newY) {
    return map[newY][newX] != '#';
}

// Função para posicionar um item em uma posição aleatória no mapa
void placeRandomItem1(int *x, int *y) {
    do {
        *x = rand() % WIDTH;
        *y = rand() % HEIGHT;
    } while (map[*y][*x] == '#' || (playerX == *x && playerY == *y));
}

// Função para inicializar o jogo, incluindo os itens e reset dos caçadores
void initializeGame1() {
    srand(time(NULL));
    placeRandomItem1(&keyX, &keyY);
    placeRandomItem1(&gasX, &gasY);
    placeRandomItem1(&carX, &carY);
    for (int i = 0; i < MAX_HUNTERS_FACIL; i++) {
        hunters1[i].active = 0; // Inicialmente, todos os caçadores estão inativos
    }
}

// Função que adiciona novos caçadores a cada 5 segundos em uma posição aleatória
void* spawnHunter1(void* arg) {
    while (!gameOver) {
        sleep(5); // Aguarda 5 segundos para criar um novo caçador
        for (int i = 0; i < MAX_HUNTERS_FACIL; i++) {
            if (!hunters1[i].active) {
                placeRandomItem1(&hunters1[i].x, &hunters1[i].y);
                hunters1[i].active = 1; // Ativa o novo caçador
                break;
            }
        }
    }
    return NULL;
}

// Função que renderiza o mapa e todos os elementos (jogador, caçadores, itens)
// Função que renderiza o mapa e todos os elementos (jogador, caçadores, itens) no modo fácil
// Função que renderiza o mapa e todos os elementos (jogador, caçadores, itens) no modo fácil
void renderMap1() {
    printf("\033[2J\033[H"); // Limpa a tela e posiciona o cursor no canto superior esquerdo
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int termWidth = w.ws_col;
    int termHeight = w.ws_row;
    int marginX = (termWidth - WIDTH) / 2;
    int marginY = (termHeight - HEIGHT - 3) / 2; // Ajuste para espaço do HUD

    // Adiciona margem vertical antes do HUD
    for (int y = 0; y < marginY - 1; y++) {
        printf("\n");
    }

    // Exibe o HUD
    for (int i = 0; i < marginX; i++) printf(" ");
    printf("🔑 %s | ⛽ %s\n",
           hasKey ? "Coletada" : "Não coletada",
           hasGasoline ? "Coletada" : "Não coletada");

    // Renderiza o mapa
    for (int y = 0; y < HEIGHT; y++) {
        // Adiciona margem horizontal
        for (int i = 0; i < marginX; i++) {
            printf(" ");
        }
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("P"); // Representação do jogador
            } else {
                int isHunter = 0;
                for (int i = 0; i < MAX_HUNTERS_FACIL; i++) {
                    if (hunters1[i].active && x == hunters1[i].x && y == hunters1[i].y) {
                        printf("H"); // Representação do caçador
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
                        printf("\033[40m \033[0m"); // Pinta a parede com fundo preto
                    } else {
                        printf(" "); // Espaço vazio
                    }
                }
            }
        }
        printf("\n");
    }

    fflush(stdout);
}


// Função que verifica se o jogador coletou algum item
void checkForItems1() {
    if (playerX == keyX && playerY == keyY && !hasKey) {
        hasKey = 1;
        printf("Você pegou a chave!\n");
    }
    if (playerX == gasX && playerY == gasY && !hasGasoline) {
        hasGasoline = 1;
        printf("Você pegou a gasolina!\n");
    }
    if (playerX == carX && playerY == carY && hasKey && hasGasoline) {
        victoryScreen1(); // Chama a função de vitória
    }
}

// Função para mover todos os caçadores em direção ao jogador
void moveHunters1() {
    for (int i = 0; i < MAX_HUNTERS_FACIL; i++) {
        if (hunters1[i].active) {
            int dx = 0, dy = 0;

            if (hunters1[i].x < playerX) dx = 1;
            else if (hunters1[i].x > playerX) dx = -1;

            if (hunters1[i].y < playerY) dy = 1;
            else if (hunters1[i].y > playerY) dy = -1;

            if (canMove1(hunters1[i].x + dx, hunters1[i].y)) hunters1[i].x += dx;
            if (canMove1(hunters1[i].x, hunters1[i].y + dy)) hunters1[i].y += dy;

            // Verifica colisão entre o caçador e o jogador
            if (hunters1[i].x == playerX && hunters1[i].y == playerY) {
                gameOverScreen1();
            }
        }
    }
}

// Função para mover o jogador
void movePlayer1(char direction) {
    int newX = playerX;
    int newY = playerY;

    switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
    }

    if (canMove1(newX, newY)) {
        playerX = newX;
        playerY = newY;
        checkForItems1(); // Verifica se o jogador coletou algum item
    } else {
        printf("\nColisão detectada! Não é possível atravessar a parede.\n");
    }
}

// Função de tela de vitória
void victoryScreen1() {
    screenDestroy();
    screenClear();
    printf("\033[2J\033[H");
    
    printf("###########################\n");
    printf("#         VITÓRIA         #\n");
    printf("###########################\n");
    printf("Pressione uma tecla para encerrar o jogo!\n");
    printf("\033[0m");
    printf("\033[?25h");
   
    exit(0);
}

// Função de tela de Game Over
void gameOverScreen1() {
    gameOver=1;
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

// Função da thread para movimentar os caçadores
void* hunterMovement1(void* arg) {
    while (!gameOver) {
        moveHunters1();
        renderMap1();
        sleep(1);
    }
    return NULL;
}

// Função principal do jogo no modo médio
void startGameFacil() {
    gameOver = 0;
    initializeGame1();
    keyboardInit();

    pthread_t hunterThread, spawnThread;
    if (pthread_create(&hunterThread, NULL, hunterMovement1, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread de movimentação dos caçadores.\n");
        exit(1);
    }
    if (pthread_create(&spawnThread, NULL, spawnHunter1, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread de spawn dos caçadores.\n");
        exit(1);
    }

    char input;
    while (1) {
        renderMap1();
        scanf(" %c", &input);
        movePlayer1(input);
        if(gameOver){
            break;
        }
      
    }

    pthread_join(hunterThread, NULL);
    pthread_join(spawnThread, NULL);
}
