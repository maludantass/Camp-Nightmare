#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "map.h"  // Inclua o cabeçalho map.h

int gameOver = 0; // Inicializa a variável gameOver como 0

// Representação do mapa maior
char map[HEIGHT][WIDTH] = {
    "############################################################",
    "#                                                          #",
    "#    #####       ########               #########          #",
    "#       #             #                                  ###",
    "#   ######          ###                 #####              #",
    "#                                                          #",
    "#        ##########             #######          #######   #",
    "#                                                          #",
    "#       ###                                               ##",
    "#   ######       ######                    ########        #",
    "#                      ###########                         #",
    "#   ####     #######                                     ###",
    "#        ##             #####                  ######       #",
    "#   ###########################         #########           #",
    "#                                                          #",
    "#    ###        ##########                                 #",
    "#                                                          #",
    "#         ######        #######             #####          #",
    "#                                                          #",
    "#                                                          #",
    "#        ####          ###                     ###         #",
    "#    #######           ###                     ###         #",
    "#          #           ###                                 #",
    "#          #                    ####                       #",
    "#          #                    ####     #######           #",
    "#      #####                    ####     #######           #",
    "#                                                          #",
    "#                     ###########                          #",
    "#                                                          #",
    "############################################################"
};

// Posições iniciais do jogador e do caçador
int playerX = 1;
int playerY = 1;
int hunterX = WIDTH - 2;
int hunterY = HEIGHT - 2;

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
        // Renderiza o conteúdo do mapa com o jogador e o caçador
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("P"); // Representação do jogador
            } else if (x == hunterX && y == hunterY) {
                printf("H"); // Representação do caçador
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
    } else {
        printf("\nColisão detectada! Não é possível atravessar a parede.\n");
    }
}

// Função para exibir a tela de "Game Over" e encerrar o jogo
void gameOverScreen() {
    printf("\033[2J\033[H"); // Limpa a tela
    printf("###########################\n");
    printf("#        GAME OVER        #\n");
    printf("###########################\n");
    printf("Pressione uma tecla pra encerrar o jogo!\n");

    // Restaura o estado do terminal
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

