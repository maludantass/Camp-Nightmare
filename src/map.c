#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#define WIDTH 60
#define HEIGHT 30

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

void* hunterMovement(void* arg) {
    while (1) {
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
    while (1) {
        renderMap();
        scanf(" %c", &input);
        movePlayer(input);
        usleep(50000);
    }

    pthread_join(hunterThread, NULL);
}

