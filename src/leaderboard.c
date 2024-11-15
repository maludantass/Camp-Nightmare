#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaderboard.h"

// Função para adicionar um jogador à lista de forma ordenada
void add_jogador(struct jogador **head, char *nome, int time) {
    struct jogador *n = *head;
    struct jogador *novo = (struct jogador*) malloc(sizeof(struct jogador));
    struct jogador *anterior = NULL;

    if (!novo) {
        fprintf(stderr, "Erro ao alocar memória para novo jogador.\n");
        return;
    }

    strcpy(novo->nome, nome);
    novo->tempo = time;
    novo->next = NULL;

    if (*head == NULL) {
        *head = novo;
        return;
    }

    if ((*head)->tempo > novo->tempo) {
        novo->next = *head;
        *head = novo;
        return;
    }

    while (n != NULL && n->tempo <= novo->tempo) {
        anterior = n;
        n = n->next;
    }

    if (anterior != NULL) {
        anterior->next = novo;
    }
    novo->next = n;
}

// Função para carregar a lista de vencedores do arquivo
void loadwinnerlist(struct jogador **head) {
    FILE *list = fopen("winners.txt", "r");
    if (!list) {
        perror("Erro ao abrir o arquivo winners.txt para leitura");
        return;
    }

    char nome[20];
    int tempo;
    while (fscanf(list, "%s %d", nome, &tempo) == 2) {
        add_jogador(head, nome, tempo);
    }

    fclose(list);
}

// Função para exibir os 10 primeiros vencedores
void printwinnerlist(struct jogador *head) {
    struct jogador *n = head;
    int i = 1;
    while (n != NULL && i <= 10) {
        printf("%d. %s: %d ticks\n", i, n->nome, n->tempo);
        n = n->next;
        i++;
    }
    printf("\n");
}

// Função para escrever a lista de vencedores no arquivo
void writewinnerlist(struct jogador *head) {
    FILE *list = fopen("winners.txt", "w");
    if (!list) {
        perror("Erro ao abrir o arquivo winners.txt para escrita");
        return;
    }

    struct jogador *n = head;
    while (n != NULL) {
        fprintf(list, "%s %d\n", n->nome, n->tempo);
        n = n->next;
    }

    fclose(list);
}

// Função para liberar a memória da lista encadeada
void free_winnerlist(struct jogador *head) {
    struct jogador *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}