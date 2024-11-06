#include <stdio.h>
#include <stdlib.h>
#include "keyboard.h"
#include "menu.h"

void displayMenu() {
    system("clear"); // Limpa a tela no Linux
    printf("====================================\n");
    printf("      BEM-VINDO AO CAMP NIGHTMARE    \n");
    printf("====================================\n");
    printf("1. Iniciar Jogo\n");
    printf("2. Carregar Jogo\n");
    printf("3. Opções\n");
    printf("4. Sair\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
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
                    printf("\nAbrindo opções...\n");
                    // Chamar função de opções
                    break;
                case '4':
                    printf("\nSaindo...\n");
                    keyboardDestroy();
                    exit(0);
                default:
                    printf("\nOpção inválida. Tente novamente.\n");
                    break;
            }
            getchar(); // Pausa para que o usuário veja a mensagem antes de voltar ao menu
        }
    }

    keyboardDestroy(); // Restaura as configurações do teclado antes de sair
}
