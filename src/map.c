#define _XOPEN_SOURCE 500  // Garante que o usleep seja reconhecido
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include "map.h"  // Inclua o cabeçalho map.h #

int gameOver = 0; // Inicializa a variável gameOver como 0
int hasKey = 0;   // Variável para verificar se o jogador pegou a chave
int hasGasoline = 0;  // Variável para verificar se o jogador pegou a gasolina

// Inicialização da matriz do mapa
char map[HEIGHT][WIDTH] = {
    "############################################################",
    "#                                                          #",
    "#   #####          ######         #######      #######     #",
    "#   #   #         . #    #         .     #      #     #    #",
    "#   #   ##########     #######    # #####   #####    ###   #",
    "#   #          #         #   #    #     #   #              #",
    "#   #######    #   #######   #    #######   #######   #### #",
    "#       #      #       #     #####      #####    ####.     #",
    "#   #####      #####          .     #####        #         #",
    "#   #          #   ####.###### #####           #####       #",
    "#   #          #   #                #      #######   ###   #",
    "#   #####      ### ##########   #####   ##         #       #",
    "#       #          .                #   #   ####### ####   #",
    "#   #####    #######     #########  #####  ###           ###",
    "#          ###     #######         #       # ####        #",
    "#   #######     #   #      #   ########     .  #    ###   ##",
    "#       #  ###  #   #      #######          # ####   #     #",
    "#   #####  #    #    #####.    ######      #       ###     #",
    "#   #        #      #       ##       .##  #         #      #",
    "############################################################"
};


// Posições iniciais do jogador, caçador, chave, gasolina e carro
int playerX = 1;
int playerY = 1;
int hunterX = WIDTH - 2;
int hunterY = HEIGHT - 2;
int keyX, keyY, gasX, gasY, carX, carY;

void placeRandomItem(int *x, int *y) {
    do {
        *x = rand() % WIDTH;
        *y = rand() % HEIGHT;
    } while (map[*y][*x] == '#' || (playerX == *x && playerY == *y));
}

void initializeGame() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    placeRandomItem(&keyX, &keyY);   // Posiciona a chave
    placeRandomItem(&gasX, &gasY);   // Posiciona a gasolina
    placeRandomItem(&carX, &carY);   // Posiciona o carro
}

void renderMap() {
    printf("\033[2J\033[H"); // Limpa a tela e move o cursor para o canto superior esquerdo

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int termWidth = w.ws_col;    // Largura atual do terminal
    int termHeight = w.ws_row;   // Altura atual do terminal

    int marginX = (termWidth - WIDTH) / 2;
    int marginY = (termHeight - HEIGHT) / 2;

    // Adiciona linhas em branco no topo para centralizar verticalmente
    for (int y = 0; y < marginY; y++) {
        printf("\n");
    }

    for (int y = 0; y < HEIGHT; y++) {
        // Adiciona espaços em branco para centralizar horizontalmente
        for (int x = 0; x < marginX; x++) {
            printf(" ");
        }
        // Renderiza o conteúdo do mapa com o jogador, caçador e itens
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("P"); // Representação do jogador
            } else if (x == hunterX && y == hunterY) {
                printf("H"); // Representação do caçador
            } else if (x == keyX && y == keyY && !hasKey) {
                printf("K"); // Representação da chave
            } else if (x == gasX && y == gasY && !hasGasoline) {
                printf("G"); // Representação da gasolina
            } else if (x == carX && y == carY) {
                printf("C"); // Representação do carro
            } else {
                printf("%c", map[y][x]);
            }
        }
        printf("\n");
    }
    fflush(stdout); // Garante que o conteúdo seja impresso no terminal
}

int canMove(int newX, int newY) {
    return map[newY][newX] != '#';
}

void victoryScreen() {
    printf("\033[2J\033[H"); // Limpa a tela
    printf("###########################\n");
    printf("#        GAnhou        #\n");
    printf("###########################\n");
    printf("Pressione uma tecla pra encerrar o jogo!\n");

    printf("\033[0m");     // Restaura as configurações padrão do terminal
    printf("\033[?25h");   // Mostra o cursor (caso tenha sido escondido)
    fflush(stdout);

    exit(0); // Encerra o programa imediatamente
   
}



void checkForItems() {
    // Verifica se o jogador pegou a chave
    if (playerX == keyX && playerY == keyY && !hasKey) {
        hasKey = 1;
        printf("Você pegou a chave!\n");
    }
    // Verifica se o jogador pegou a gasolina
    if (playerX == gasX && playerY == gasY && !hasGasoline) {
        hasGasoline = 1;
        printf("Você pegou a gasolina!\n");
    }
    // Verifica condição de vitória e chama a tela de vitória imediatamente
    if (playerX == carX && playerY == carY && hasKey && hasGasoline) {
        victoryScreen();  // Exibe a tela de vitória e encerra o jogo
    }
}


void moveHunter() {
    int dx = 0, dy = 0;

    if (hunterX < playerX) dx = 1;
    else if (hunterX > playerX) dx = -1;

    if (hunterY < playerY) dy = 1;
    else if (hunterY > playerY) dy = -1;

    if (canMove(hunterX + dx, hunterY)) hunterX += dx;
    if (canMove(hunterX, hunterY + dy)) hunterY += dy;

    // Verifica colisão entre o caçador e o jogador
    if (hunterX == playerX && hunterY == playerY) {
        gameOverScreen(); // Chama a função Game Over imediatamente ao detectar colisão
    }
}

void movePlayer(char direction) {
    int newX = playerX;
    int newY = playerY;

    switch (direction) {
        case 'w': newY--; break; // Cima
        case 's': newY++; break; // Baixo
        case 'a': newX--; break; // Esquerda
        case 'd': newX++; break; // Direita
    }

    if (canMove(newX, newY)) {
        playerX = newX;
        playerY = newY;
        checkForItems(); // Verifica se o jogador coletou algum item
    } else {
        printf("\nColisão detectada! Não é possível atravessar a parede.\n");
    }
}

void gameOverScreen() {
    printf("\033[2J\033[H"); // Limpa a tela
    printf("###########################\n");
    printf("#        GAME OVER        #\n");
    printf("###########################\n");
    printf("Pressione uma tecla pra encerrar o jogo!\n");

    printf("\033[0m");     // Restaura as configurações padrão do terminal
    printf("\033[?25h");   // Mostra o cursor (caso tenha sido escondido)
    fflush(stdout);

    exit(0); // Encerra o programa imediatamente
}

void* hunterMovement(void* arg) {
    while (!gameOver) {
        moveHunter();
        renderMap();
        sleep(1);
    }
    return NULL;
}

void startGame() {
    initializeGame();  // Inicializa os itens no mapa

    pthread_t hunterThread;
    if (pthread_create(&hunterThread, NULL, hunterMovement, NULL) != 0) {
        fprintf(stderr, "Erro ao criar a thread do caçador.\n");
        exit(1);
    }

    char input;
    while (!gameOver) {
        renderMap();
        scanf(" %c", &input);
        movePlayer(input);
        usleep(50000);
    }
}