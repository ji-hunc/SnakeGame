#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <iostream>
#include "Map.h"
#include <thread>
// #include "Snake.h"

using namespace std;
// 방향키 input은 전역변수로
int userInput;
int appleCount;
int poisonCount;

int main() {
    Map *map = new Map();
    Snake *snake = new Snake('l'); // 처음엔 왼쪽방향으로 출발
    

    setlocale(LC_ALL, "");
    initscr();
    start_color();
    init_color(100, 1000, 1000, 1000); // plain white

    init_color(1, 950, 970, 1000); // floor
    init_color(2, 10, 680, 980); // wall
    init_color(3, 10, 560, 800); // corner
    init_color(4, 1000, 800, 180); // snake head
    init_color(5, 1000, 960, 240); // snake body
    init_color(6, 0, 0, 0); // gate
    init_color(7, 400, 250, 700); // snake tail
    init_color(8, 1000, 400, 250); // apple
    init_color(9, 400, 1000, 200); // poison
    init_pair(10, 100, COLOR_BLACK); // scoreboard
    init_pair(1, 1, 1);
    init_pair(2, 2, 2);
    init_pair(3, 3, 3);
    init_pair(4, 4, 4);
    init_pair(5, 5, 5);
    init_pair(6, 6, 6);
    init_pair(7, 7, 7);
    init_pair(8, 8, 8);
    init_pair(9, 9, 9);
    
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    // init map
    map->initMap();

    

    while (true) {
        map->updateSnake(*snake);
        map->getInput(*snake);
        map->generateApple(*snake);
        map->generatePoison(*snake);
        map->generateGate();
        map->updateScoreBoard(*snake);
    }
    getch();
    endwin();
}