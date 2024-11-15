#ifndef LEADERBOARD_H
#define LEADERBOARD_H

struct jogador {
    char nome[50];
    int tempo;
    struct jogador *next;
};

// Funções para manipular a lista de classificação
void add_jogador(struct jogador **head, char *nome, int time);
void loadwinnerlist(struct jogador **head);
void printwinnerlist(struct jogador *head);
void writewinnerlist(struct jogador *head);
void free_winnerlist(struct jogador *head);

#endif // LEADERBOARD_H