/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include <stdio.h>
#include "menu.h"
#include "map.h"
#include "medio.h"
#include "facil.h"
#include "dificil.h"
#include "leaderboard.h"
int main() {
    // Inicia o menu do jogo "Camp Nightmare"
    runMenu();
    
    return 0;
} 
