#include <stdio.h>
#include <stdlib.h>
#include "keyboard.h"
#include "menu.h"
#include "leaderboard.h" // Inclui o header para acessar as funções do ranking
#include "screen.h"


#include <termios.h>  // Necessário para tcflush e manipulação do terminal
#include <unistd.h>   // Para STDIN_FILENO
#include <stdio.h>    // Para entrada e saída padrão (printf, getchar)
#include "keyboard.h" // Suas funções de teclado
#include "menu.h"     // Declarações relacionadas ao menu


void displayMenu() {
    // Dimensões do terminal assumidas (135x36)
    int terminalWidth = 135;
    int terminalHeight = 36;

    // Espaçamento ajustado para centralizar verticalmente
    int verticalPadding = ((terminalHeight - 14) / 2)-4; // Mantém o centro relativo ao terminal

    // Espaçamento para centralizar horizontalmente
    int horizontalPadding = (terminalWidth - 40) / 2;

    // Limpa a tela
    system("clear");

    // Adiciona linhas vazias para ajustar verticalmente
    for (int i = 0; i < verticalPadding; i++) {
        printf("\n");
    }

    // Função auxiliar para adicionar espaços horizontais
    void printHorizontalPadding() {
        for (int i = 0; i < horizontalPadding; i++) {
            printf(" ");
        }
    }

    // Exibe o menu com padding horizontal
   
   

    printHorizontalPadding();
    printf("  / ___|__ _ _ __ ___  _ __   \n");
    printHorizontalPadding();
    printf(" | |   / _` | '_ ` _ \\| '_ \\ \n");
    printHorizontalPadding();
    printf(" | |__| (_| | | | | | | |_) | \n");
    printHorizontalPadding();
    printf("  \\____\\__,_|_| |_| |_| .__/ \n");
    printHorizontalPadding();
    printf("                      |_|    \n");
    printHorizontalPadding();
    printf("  _   _ _       _     _      \n");
    printHorizontalPadding();
    printf(" | \\ | (_) __ _| |__ | |_ _ __ ___   __ _ _ __ ___\n");
    printHorizontalPadding();
    printf(" |  \\| | |/ _` | '_ \\| __| '_ ` _ \\ / _` | '__/ _ \\\n");
    printHorizontalPadding();
    printf(" | |\\  | | (_| | | | | |_| | | | | | (_| | | |  __/\n");
    printHorizontalPadding();
    printf(" |_| \\_|_|\\__, |_| |_|\\__|_| |_| |_|\\__,_|_|  \\___|\n");
    printHorizontalPadding();
    printf("          |___/                                   \n");

    printHorizontalPadding();
    printf("\n");
   

    printHorizontalPadding();
    printf("1. Modos de Jogo\n");
    printHorizontalPadding();
    printf("2. Sair\n");
    printHorizontalPadding();
    printf("3. Ver Ranking\n");

    
    

    
    printf("\nEscolha uma opção: ");
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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Consome todos os caracteres no buffer
}


void runMenu() {
    keyboardInit(); // Inicializa as configurações do teclado
    
    int choice;

    while (1) {
        // Limpa o buffer de entrada para evitar processar dados residuais
        while (keyhit()) {
            readch(); // Consome qualquer caractere no buffer
        }

        displayMenu();

        // Aguarda a entrada do usuário
        choice = readch(); // Lê o caractere diretamente

        switch (choice) {
            case '1':
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
            case '2':
                printf("\nSaindo...\n");
                keyboardDestroy();
                exit(0);
            case '3':
                exibirRanking(); // Chama a função para exibir o ranking
                break;
            default:
                printf("\n\nOpção inválida. Tente novamente.\n");
                getchar(); // Pausa para que o usuário veja a mensagem antes de voltar ao menu
                break;
        }
    }

    keyboardDestroy(); // Restaura as configurações do teclado antes de sair
}
