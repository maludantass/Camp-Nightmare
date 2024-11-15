#include <stdio.h>
#include <stdlib.h>
#include "keyboard.h"
#include "menu.h"
#include "leaderboard.h" // Inclui o header para acessar as funções do ranking

void displayMenu() {
    system("clear"); // Limpa a tela no Linux
    printf("====================================\n");
    printf("      BEM-VINDO AO CAMP NIGHTMARE   \n");
    printf("====================================\n");
    printf("1. Iniciar Jogo\n");
    printf("2. Carregar Jogo\n");
    printf("3. Opções\n");
    printf("4. Sair\n");
    printf("5. Ver Ranking\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
}

void displayDifficultyMenu() {
    system("clear");
    printf("====================================\n");
    printf("        SELECIONE A DIFICULDADE      \n");
    printf("====================================\n");
    printf("1. Fácil\n");
    printf("2. Médio\n");
    printf("3. Difícil\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
}

// Função para exibir o ranking
void exibirRanking() {
    struct jogador *head = NULL;
    loadwinnerlist(&head); // Carrega o ranking do arquivo
    system("clear");
    printf("====================================\n");
    printf("            RANKING                 \n");
    printf("====================================\n");
    printwinnerlist(head); // Exibe os 10 melhores jogadores
    free_winnerlist(head); // Libera a memória alocada para a lista
    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    getchar(); // Pausa antes de voltar ao menu principal
}

void runMenu() {
    keyboardInit(); // Inicializa as configurações do teclado
    int choice;

    while (1) {
        displayMenu();
        if (keyhit()) {
            choice = readch();
            switch (choice) {
                case '1':
                    printf("\nIniciando o jogo...\n");
                    startGame(); // Chama a função de início de jogo
                    break;
                case '2':
                    printf("\nCarregando o jogo...\n");
                    // Chamar função de carregamento
                    break;
                case '3':
                    displayDifficultyMenu();
                    int difficultyChoice = readch();
                    switch (difficultyChoice) {
                        case '1':
                            printf("\nDificuldade selecionada: Fácil\n");
                            startGameFacil();
                            break;
                        case '2':
                            printf("\nDificuldade selecionada: Médio\n");
                            startGameMedio();
                            break;
                        case '3':
                            printf("\nDificuldade selecionada: Difícil\n");
                            startGameDificil();
                            break;
                        default:
                            printf("\nOpção inválida. Retornando ao menu principal.\n");
                            break;
                    }
                    getchar(); // Pausa para que o usuário veja a mensagem antes de voltar ao menu
                    break;
                case '4':
                    printf("\nSaindo...\n");
                    keyboardDestroy();
                    exit(0);
                case '5':
                    exibirRanking(); // Chama a função para exibir o ranking
                    break;
                default:
                    printf("\nOpção inválida. Tente novamente.\n");
                    break;
            }
            getchar(); // Pausa para que o usuário veja a mensagem antes de voltar ao menu
        }
    }

    keyboardDestroy(); // Restaura as configurações do teclado antes de sair
}