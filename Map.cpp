#include <iostream>
#include "Map.h"
#include <vector>
#include <time.h>
// #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

extern int userInput;
extern int appleCount;
extern int poisonCount;
extern int gateRow1;
extern int gateRow2;
extern int gateCol1;
extern int gateCol2;
time_t appleStart;
time_t poisonStart;
time_t gateStart = time(NULL);
bool findWall = false;


void Map::initMap() {
    for (int i=0; i<25; i++) {
        for (int j=0; j<50; j++) {
            if (board[i][j] == '0') {
                attron(COLOR_PAIR(1));
                printw("0");
                attroff(COLOR_PAIR(1));
            }
            else if (board[i][j] == '1') {
                attron(COLOR_PAIR(2));
                printw("1");
                attroff(COLOR_PAIR(2));
            }
            else {
                attron(COLOR_PAIR(3));
                printw("2");
                attroff(COLOR_PAIR(3));
            }
        }
        printw("\n");
    }
    refresh();
}

bool Map::isCrashWithWall(Position head) {
    int headRow = head.row;
    int headCol = head.col;
    return board[headRow][headCol] == '1';
}
// static time_t input_time = time(NULL);

void Map::getInput(Snake &snake) {
    // **** 입력을 안 하면 그냥 넘어감 halfdelay() ****
    halfdelay(1);
    userInput = getch();

    if (userInput == KEY_UP) {
        if (snake.getDirection() != 'd') {
            snake.setDirection('u');
        }
        else {
            cout << "GAME OVER" << endl;
            terminate();
        }
    }
    else if (userInput == KEY_DOWN) {
        if (snake.getDirection() != 'u') {
            snake.setDirection('d');
        }
        else {
            cout << "GAME OVER" << endl;
            terminate();
        }
    }
    else if (userInput == KEY_LEFT) {
        if (snake.getDirection() != 'r') {
            snake.setDirection('l');
        }
        else {
            cout << "GAME OVER" << endl;
            terminate();
        }
    }
    else if (userInput == KEY_RIGHT) {
        if (snake.getDirection() != 'l') {
            snake.setDirection('r');
        }
        else {
            cout << "GAME OVER" << endl;
            terminate();
        }
    }
}

tuple<int, int, char> Map::checkFuture(int r, int c, char dir) {
    char* direction;
    int* indexRow;
    int* indexCol;
    if (dir == 'l') {
        char di[4] = {'l', 'u', 'd', 'r'};
        int ir[4] = {0, -1, 1, 0};
        int il[4] = {-2, 0, 0, 2};

        direction = di;
        indexRow = ir;
        indexCol = il;
    }
    else if (dir == 'r') {
        char di[4] = {'r', 'd', 'u', 'l'};
        int ir[4] = {0, 1, -1, 0};
        int il[4] = {2, 0, 0, -2};
        direction = di;
        indexRow = ir;
        indexCol = il;
    }
    else if (dir == 'u') {
        char di[4] = {'u', 'r', 'l', 'd'};
        int ir[4] = {-1, 0, 0, 1};
        int il[4] = {0, 2, -2, 0};
        direction = di;
        indexRow = ir;
        indexCol = il;
    }
    else {
        char di[4] = {'d', 'l', 'r', 'u'};
        int ir[4] = {1, 0, 0, -1};
        int il[4] = {0, -2, 2, 0};
        direction = di;
        indexRow = ir;
        indexCol = il;
    }
    for (int i=0; i<4; i++) {
        if (board[r+indexRow[i]][c+indexCol[i]] == '0') {
            tuple <int, int, char> t1 = make_tuple(r+indexRow[i], c+indexCol[i], direction[i]);
            return t1;
        }
    }
}

void Map::updateSnake(Snake &snake) {
    if (snake.getDirection() == 'l') {
        snake.location.insert(snake.location.begin(), Position(snake.location[0].row, snake.location[0].col - 2));
    }
    else if (snake.getDirection() == 'r') {
        snake.location.insert(snake.location.begin(), Position(snake.location[0].row, snake.location[0].col + 2));
    }
    else if (snake.getDirection() == 'u') {
        snake.location.insert(snake.location.begin(), Position(snake.location[0].row - 1, snake.location[0].col));
    }
    else if (snake.getDirection() == 'd') {
        snake.location.insert(snake.location.begin(), Position(snake.location[0].row + 1, snake.location[0].col));
    }
    snake.location.pop_back();

    if (snake.location[0].row == gateRow1 && snake.location[0].col == gateCol1 || snake.location[0].row == gateRow2 && snake.location[0].col == gateCol2) {
        if (snake.location[0].row == gateRow1 && snake.location[0].col == gateCol1) {
            tuple<int, int, char> temp;
            temp = checkFuture(gateRow2, gateCol2, snake.getDirection());
            snake.location.insert(snake.location.begin(), Position(get<0>(temp), get<1>(temp)));
            snake.setDirection(get<2>(temp));
            snake.location.erase(snake.location.begin()+1);
        }
        if (snake.location[0].row == gateRow2 && snake.location[0].col == gateCol2) {
            tuple<int, int, char> temp;
            temp = checkFuture(gateRow1, gateCol1, snake.getDirection());
            snake.location.insert(snake.location.begin(), Position(get<0>(temp), get<1>(temp)));
            snake.setDirection(get<2>(temp));
            snake.location.erase(snake.location.begin()+1);
        }
    }

    if (snake.isCrashMySelf() || isCrashWithWall(snake.location[0])) {
        cout << "game Over" << endl;
        terminate();
    }

    int r = snake.location[0].row;
    int c = snake.location[0].col;

    if (r == appleLocation.first && c == appleLocation.second) {
        snake.eatApple(appleLocation.first, appleLocation.second);
        appleCount--;
    }
    if (r == poisonLocation.first && c == poisonLocation.second) {
        r = snake.location[snake.location.size()-1].row;
        c = snake.location[snake.location.size()-1].col;
        attron(COLOR_PAIR(1));
        board[r][c] = '0';
        board[r][c+1] = '0';
        mvprintw(r, c, "0"); 
        mvprintw(r, c+1, "0");
        attroff(COLOR_PAIR(1));
        snake.eatPoison(poisonLocation.first, poisonLocation.second);
        poisonCount--;
    }
    // 머리 출력
    r = snake.location[0].row;
    c = snake.location[0].col;
    // 머리가 아이템이랑 부딪혔나?
    attron(COLOR_PAIR(4));
    board[r][c] = '3';
    board[r][c+1] = '3';
    mvprintw(r, c, "3");
    mvprintw(r, c+1, "3");
    attroff(COLOR_PAIR(4));

    // 몸통 출력
    for (int i=1; i<snake.getLength(); i++) {
        r = snake.location[i].row;
        c = snake.location[i].col;
        attron(COLOR_PAIR(5));
        board[r][c] = '4'; 
        board[r][c+1] = '4'; 
        mvprintw(r, c, "4"); 
        mvprintw(r, c+1, "4");
        attroff(COLOR_PAIR(5));
    }

    refresh();
    // 잔상 삭제
    attron(COLOR_PAIR(1));
    board[r][c] = '0';
    board[r][c+1] = '0';
    mvprintw(r, c, "0"); 
    mvprintw(r, c+1, "0");
    attroff(COLOR_PAIR(1));

    if (snake.getLength() == 4) {
        cout << "GAME OVER" << endl;
        terminate();
    }

    // 0.5초 대기
    // usleep(500000);
}

void Map::printMap() {
    for (int i=0; i<25; i++) {
        for (int j=0; j<50; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void Map::generateApple(Snake &snake) {
    srand(time(NULL));
    bool ableToGenerate = false;
    int r, c;

    time_t now = time(NULL);

    if (appleCount == 1 && (double)(now - appleStart) > 5) {
        attron(COLOR_PAIR(1));
        r = appleLocation.first;
        c = appleLocation.second;
        board[r][c] = '0';
        board[r][c+1] = '0';
        mvprintw(r, c, "0"); 
        mvprintw(r, c+1, "0");
        attroff(COLOR_PAIR(1));
        ableToGenerate = false;
        appleCount--;
    }

    if (appleCount < 1) {
        while (!ableToGenerate) {
            r = rand() % 23 + 1;
            c = rand() % 46 + 1;
            if (c % 2 != 0) {
                c++;
            }

            for (int i=0; i<snake.location.size(); i++) {
                if (r != snake.location[i].row && c != snake.location[i].col) {
                    ableToGenerate = true;
                }
                else {
                    ableToGenerate = false;
                    break;
                }
            }
            if (r == poisonLocation.first && c == poisonLocation.second) {
                ableToGenerate = false;
            }
        }
        attron(COLOR_PAIR(4));
        board[r][c] = '5';
        board[r][c+1] = '5';
        mvprintw(r, c, "5");
        mvprintw(r, c+1, "5");
        attroff(COLOR_PAIR(4));
        appleCount++;

        appleLocation.first = r;
        appleLocation.second = c;
        appleStart = time(NULL);
    }
}

void Map::generatePoison(Snake &snake) {
    // srand(time(NULL));
    bool ableToGenerate = false;
    int r, c;

    time_t now = time(NULL);

    if (poisonCount == 1 && (double)(now - poisonStart) > 5) {
        attron(COLOR_PAIR(1));
        r = poisonLocation.first;
        c = poisonLocation.second;
        board[r][c] = '0';
        board[r][c+1] = '0';
        mvprintw(r, c, "0"); 
        mvprintw(r, c+1, "0");
        attroff(COLOR_PAIR(1));
        ableToGenerate = false;
        poisonCount--;
    }

    if (poisonCount < 1) {
        while (!ableToGenerate) {
            r = rand() % 23 + 1;
            c = rand() % 46 + 1;
            if (c % 2 != 0) {
                c++;
            }

            for (int i=0; i<snake.location.size(); i++) {
                if (r != snake.location[i].row && c != snake.location[i].col) {
                    ableToGenerate = true;
                }
                else {
                    ableToGenerate = false;
                    break;
                }
            }
            if (r == appleLocation.first && c == appleLocation.second) {
                ableToGenerate = false;
            }
        }
        attron(COLOR_PAIR(3));
        board[r][c] = '6'; 
        board[r][c+1] = '6';
        mvprintw(r, c, "6");
        mvprintw(r, c+1, "6");
        attroff(COLOR_PAIR(3));
        poisonCount++;

        poisonLocation.first = r;
        poisonLocation.second = c;
        poisonStart = time(NULL);
    }
}

void Map::generateGate() {
    srand(time(NULL));
    int x, y;
    time_t now = time(NULL);

    if (now - gateStart > 10) {
        attron(COLOR_PAIR(2));
        board[gateRow1][gateCol1] = '1';
        board[gateRow1][gateCol1+1] = '1';
        mvprintw(gateRow1, gateCol1, "1"); 
        mvprintw(gateRow1, gateCol1+1, "1");
        board[gateRow2][gateCol2] = '1';
        board[gateRow2][gateCol2+1] = '1';
        mvprintw(gateRow2, gateCol2, "1"); 
        mvprintw(gateRow2, gateCol2+1, "1");
        attroff(COLOR_PAIR(2));
        findWall = false;
    }

    while (!findWall) {
        x = rand() % 25;
        y = (rand() % 25) * 2;

        while (true) {
            x = rand() % 25;
            y = (rand() % 25) * 2;
            if (board[x][y] == '1') {
                gateRow1 = x;
                gateCol1 = y;
                break;
            }
        }
        while (true) {
            x = rand() % 25;
            y = (rand() % 25) * 2;
            if (board[x][y] == '1' && (x != gateRow1 || y != gateCol1)) {
                gateRow2 = x;
                gateCol2 = y;
                findWall = true;
                gateStart = time(NULL);
                break;
            }
        }

    }
    
    attron(COLOR_PAIR(6));
    board[gateRow1][gateCol1] = '7';
    board[gateRow1][gateCol1+1] = '7';
    mvprintw(gateRow1, gateCol1, "7"); 
    mvprintw(gateRow1, gateCol1+1, "7");
    attroff(COLOR_PAIR(6));

    attron(COLOR_PAIR(6));
    board[gateRow2][gateCol2] = '7';
    board[gateRow2][gateCol2+1] = '7';
    mvprintw(gateRow2, gateCol2, "7");
    mvprintw(gateRow2, gateCol2+1, "7");
    attroff(COLOR_PAIR(6));
}