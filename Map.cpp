#include <iostream>
#include "Map.h"
#include <vector>
#include <time.h>
// #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <unistd.h>
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
bool isPassing = false;
char outDirection;
bool exit2;
WINDOW *scoreBoard; // 점수판
WINDOW *missionBoard; // 미션판
WINDOW *complete; // 미션 성공
WINDOW *gameover; // 미션 실패
bool wGameOver = false;
int appleScore = 0;
int poisonScore = 0;
int gateScore = 0;
char lengthComplete = ' ';
char appleComplete = ' ';
char poisonComplete = ' ';
char gateComplete = ' ';
bool missionComplete = false;
#define MAX_LENGTH 5
#define MAX_APPLE 2
#define MAX_POISON 1
#define MAX_GATE 1

void Map::initMap() {
    for (int i=0; i<25; i++) {
        for (int j=0; j<50; j++) {
            if (board[stageLevel][i][j] == '0') {
                attron(COLOR_PAIR(1));  
                printw("0");
                attroff(COLOR_PAIR(1));
            }
            else if (board[stageLevel][i][j] == '1') {
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
    return board[stageLevel][headRow][headCol] == '1';
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
            wGameOver = true;
        }
    }
    else if (userInput == KEY_DOWN) {
        if (snake.getDirection() != 'u') {
            snake.setDirection('d');
        }
        else {
            wGameOver = true;
        }
    }
    else if (userInput == KEY_LEFT) {
        if (snake.getDirection() != 'r') {
            snake.setDirection('l');
        }
        else {
            wGameOver = true;
        }
    }
    else if (userInput == KEY_RIGHT) {
        if (snake.getDirection() != 'l') {
            snake.setDirection('r');
        }
        else {
            wGameOver = true;
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
        if (board[stageLevel][r+indexRow[i]][c+indexCol[i]] == '0') {
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
        isPassing = true;
        gateScore++;
        if (gateScore == MAX_GATE){
            gateComplete = 'O';
        }
        if (snake.location[0].row == gateRow1 && snake.location[0].col == gateCol1) {
            exit2 = true;
            tuple<int, int, char> temp;
            temp = checkFuture(gateRow2, gateCol2, snake.getDirection());
            snake.location.insert(snake.location.begin(), Position(get<0>(temp), get<1>(temp)));
            snake.setDirection(get<2>(temp));
            outDirection = get<2>(temp);
            snake.location.erase(snake.location.begin()+1);
        }
        if (snake.location[0].row == gateRow2 && snake.location[0].col == gateCol2) {
            exit2 = false;
            tuple<int, int, char> temp;
            temp = checkFuture(gateRow1, gateCol1, snake.getDirection());
            snake.location.insert(snake.location.begin(), Position(get<0>(temp), get<1>(temp)));
            snake.setDirection(get<2>(temp));
            outDirection = get<2>(temp);
            snake.location.erase(snake.location.begin()+1);
        }
    }
    if (isPassing) {
        if (!exit2) {
            if (outDirection == 'l' && board[stageLevel][gateRow1][gateCol1-2] == '8') {
                isPassing = false;
            } 
            else if (outDirection == 'r' && board[stageLevel][gateRow1][gateCol1+2] == '8') {
                isPassing = false;
            } 
            else if (outDirection == 'u' && board[stageLevel][gateRow1-1][gateCol1] == '8') {
                isPassing = false;
            } 
            else if (outDirection == 'd' && board[stageLevel][gateRow1+1][gateCol1] == '8'){
                isPassing = false;
            }
        }
        else {
            if (outDirection == 'l' && board[stageLevel][gateRow2][gateCol2-2] == '8') {
                isPassing = false;
            } 
            else if (outDirection == 'r' && board[stageLevel][gateRow2][gateCol2+2] == '8') {
                isPassing = false;
            } 
            else if (outDirection == 'u' && board[stageLevel][gateRow2-1][gateCol2] == '8') {
                isPassing = false;
            } 
            else if (outDirection == 'd' && board[stageLevel][gateRow2+1][gateCol2] == '8'){
                isPassing = false;
            }
        }
    }


    if (snake.isCrashMySelf() || isCrashWithWall(snake.location[0]) || wGameOver) {
        gameover = newwin(10, 35, 8, 8);
        wbkgd(gameover, COLOR_PAIR(10));
        mvwprintw(gameover, 3, 12, "Game Over!!!");
        mvwprintw(gameover, 5, 6, "Press Enter to quit game");
        wrefresh(gameover);
        keypad(complete, TRUE);
        curs_set(0);
        noecho();
        scanw("");
        clear();
        delwin(gameover);
        endwin();


    int r = snake.location[0].row;
    int c = snake.location[0].col;

    // eat Apple
    if (r == appleLocation.first && c == appleLocation.second) {
        snake.eatApple(appleLocation.first, appleLocation.second);
        appleCount--;
        appleScore++;
        if (appleScore == MAX_APPLE){
            appleComplete = 'O';
        }
    }
    // eat Poison
    if (r == poisonLocation.first && c == poisonLocation.second) {
        r = snake.location[snake.location.size()-1].row;
        c = snake.location[snake.location.size()-1].col;
        attron(COLOR_PAIR(1));
        board[stageLevel][r][c] = '0';
        board[stageLevel][r][c+1] = '0';
        mvprintw(r, c, "0"); 
        mvprintw(r, c+1, "0");
        attroff(COLOR_PAIR(1));
        snake.eatPoison(poisonLocation.first, poisonLocation.second);
        poisonCount--;
        poisonScore++;
        if (poisonScore == MAX_POISON){
            poisonComplete = 'O';
        }
    }
    // 머리 출력
    r = snake.location[0].row;
    c = snake.location[0].col;
    // 머리가 아이템이랑 부딪혔나?
    attron(COLOR_PAIR(4));
    board[stageLevel][r][c] = '3';
    board[stageLevel][r][c+1] = '3';
    mvprintw(r, c, "3");
    mvprintw(r, c+1, "3");
    attroff(COLOR_PAIR(4));

    // 몸통 출력
    // for (int i=1; i<snake.getLength(); i++) {
    //     r = snake.location[i].row;
    //     c = snake.location[i].col;
    //     attron(COLOR_PAIR(5));
    //     board[stageLevel][r][c] = '4'; 
    //     board[stageLevel][r][c+1] = '4'; 
    //     mvprintw(r, c, "4"); 
    //     mvprintw(r, c+1, "4");
    //     attroff(COLOR_PAIR(5));
    // }

    // 몸통에서 꼬리만 뺴고 출력
    for (int i=1; i<snake.getLength()-1; i++) {
        r = snake.location[i].row;
        c = snake.location[i].col;
        attron(COLOR_PAIR(5));
        board[stageLevel][r][c] = '4'; 
        board[stageLevel][r][c+1] = '4'; 
        mvprintw(r, c, "4"); 
        mvprintw(r, c+1, "4");
        attroff(COLOR_PAIR(5));
    }
    // 꼬리만 출략
    r = snake.location[snake.location.size()-2].row;
    c = snake.location[snake.location.size()-2].col;
    attron(COLOR_PAIR(7));
    board[stageLevel][r][c] = '8';
    board[stageLevel][r][c+1] = '8';
    mvprintw(r, c, "8");
    mvprintw(r, c+1, "8");
    attroff(COLOR_PAIR(7));



    refresh();
    // // 잔상 삭제
    // attron(COLOR_PAIR(1));
    // board[stageLevel][r][c] = '0';
    // board[stageLevel][r][c+1] = '0';
    // mvprintw(r, c, "0"); 
    // mvprintw(r, c+1, "0");
    // attroff(COLOR_PAIR(1));
    // 잔상 삭제
    r = snake.location[snake.location.size()-1].row;
    c = snake.location[snake.location.size()-1].col;
    attron(COLOR_PAIR(1));
    board[stageLevel][r][c] = '0';
    board[stageLevel][r][c+1] = '0';
    mvprintw(r, c, "0"); 
    mvprintw(r, c+1, "0");
    attroff(COLOR_PAIR(1));
    

    if (snake.getLength() == 4) {
        wGameOver = true;
        // terminate();
    }
    if (snake.getLength() >= MAX_LENGTH){
        lengthComplete = 'O';
    }
    else{
        lengthComplete = ' ';
    }

    // 0.5초 대기
    // usleep(500000);
}

void Map::printMap() {
    for (int i=0; i<25; i++) {
        for (int j=0; j<50; j++) {
            cout << board[stageLevel][i][j];
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
        board[stageLevel][r][c] = '0';
        board[stageLevel][r][c+1] = '0';
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
            if (board[stageLevel][r][c] != '0') {
                continue;
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
        attron(COLOR_PAIR(8));
        board[stageLevel][r][c] = '5';
        board[stageLevel][r][c+1] = '5';
        mvprintw(r, c, "5");
        mvprintw(r, c+1, "5");
        attroff(COLOR_PAIR(8));
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
        board[stageLevel][r][c] = '0';
        board[stageLevel][r][c+1] = '0';
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
            if (board[stageLevel][r][c] != '0') {
                continue;
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
        attron(COLOR_PAIR(9));
        board[stageLevel][r][c] = '6'; 
        board[stageLevel][r][c+1] = '6';
        mvprintw(r, c, "6");
        mvprintw(r, c+1, "6");
        attroff(COLOR_PAIR(9));
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
    if (now - gateStart > 5 && !isPassing) {
        attron(COLOR_PAIR(2));
        board[stageLevel][gateRow1][gateCol1] = '1';
        board[stageLevel][gateRow1][gateCol1+1] = '1';
        mvprintw(gateRow1, gateCol1, "1");
        mvprintw(gateRow1, gateCol1+1, "1");
        board[stageLevel][gateRow2][gateCol2] = '1';
        board[stageLevel][gateRow2][gateCol2+1] = '1';
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
            if (board[stageLevel][x][y] == '1') {
                gateRow1 = x;
                gateCol1 = y;
                break;
            }
        }
        while (true) {
            x = rand() % 25;
            y = (rand() % 25) * 2;
            if (board[stageLevel][x][y] == '1' && (x != gateRow1 || y != gateCol1)) {
                gateRow2 = x;
                gateCol2 = y;
                findWall = true;
                gateStart = time(NULL);
                break;
            }
        }

    }
    
    attron(COLOR_PAIR(6));
    board[stageLevel][gateRow1][gateCol1] = '7';
    board[stageLevel][gateRow1][gateCol1+1] = '7';
    mvprintw(gateRow1, gateCol1, "7"); 
    mvprintw(gateRow1, gateCol1+1, "7");
    attroff(COLOR_PAIR(6));

    attron(COLOR_PAIR(6));
    board[stageLevel][gateRow2][gateCol2] = '7';
    board[stageLevel][gateRow2][gateCol2+1] = '7';
    mvprintw(gateRow2, gateCol2, "7");
    mvprintw(gateRow2, gateCol2+1, "7");
    attroff(COLOR_PAIR(6));
}
void Map::updateScoreBoard(Snake &snake){
    scoreBoard = newwin(10, 30, 2, 55);
    wbkgd(scoreBoard, COLOR_PAIR(10));
    wattron(scoreBoard, COLOR_PAIR(10));
    wborder(scoreBoard, '.','.','-','-','o','o','o','o');
    mvwprintw(scoreBoard, 2, 9, "Score Board");
    mvwprintw(scoreBoard, 4, 10, "B : %d / %d", snake.getLength(), MAX_LENGTH);
    mvwprintw(scoreBoard, 5, 12, "+ : %d", appleScore);
    mvwprintw(scoreBoard, 6, 12, "- : %d", poisonScore);
    mvwprintw(scoreBoard, 7, 12, "G : %d", gateScore);

    missionBoard = newwin(10, 30, 13, 55);
    wattron(missionBoard, COLOR_PAIR(10));
    wborder(missionBoard, '.', '.', '-', '-', 'o', 'o', 'o', 'o');
    mvwprintw(missionBoard, 2, 9, "Mission Board");
    mvwprintw(missionBoard, 4, 11, "B : %d (%c)", MAX_LENGTH, lengthComplete);
    mvwprintw(missionBoard, 5, 11, "+ : %d (%c)", MAX_APPLE, appleComplete);
    mvwprintw(missionBoard, 6, 11, "- : %d (%c)", MAX_POISON, poisonComplete);
    mvwprintw(missionBoard, 7, 11, "G : %d (%c)", MAX_GATE, gateComplete);

    complete = newwin(10, 35, 8, 8);
    wattron(complete, COLOR_PAIR(10));
    wborder(complete, '.', '.', '-', '-', 'o', 'o', 'o', 'o');
    mvwprintw(complete, 3, 8, "Mission Complete!!!");
    mvwprintw(complete, 5, 3, "Press Enter to continue game");

    wrefresh(scoreBoard);
    wrefresh(missionBoard);
    missionComplete = lengthComplete == 'O' && appleComplete == 'O' && poisonComplete == 'O' && gateComplete == 'O';
    

    if (missionComplete){
        if (stageLevel == 3) {
            terminate();
        }
        int continueInput = 0;
        stageLevel++;
        wrefresh(complete);
        keypad(complete, TRUE);
        curs_set(0);
        noecho();
        scanw("");

        
        delwin(complete);
        endwin();
        clear();
        snake.initSnake('l');
        initMap();

        appleScore = 0;
        poisonScore = 0;
        gateScore = 0;
        lengthComplete = ' ';
        appleComplete = ' ';
        poisonComplete = ' ';
        gateComplete = ' ';
        isPassing = false;
        appleCount = 0;
        poisonCount = 0;

    }
    
    
}