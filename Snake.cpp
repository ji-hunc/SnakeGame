#include <iostream>
#include "Snake.h"
using namespace std;

Snake::Snake(char dir) {
    direction = dir;
    location.push_back(Position(3, 24));
    location.push_back(Position(3, 26));
    location.push_back(Position(3, 28));
    location.push_back(Position(3, 30));
    location.push_back(Position(3, 32));
    location.push_back(Position(3, 34));
}
void Snake::initSnake(char dir) {
    direction = dir;
    location.clear();
    location.push_back(Position(3, 24));
    location.push_back(Position(3, 26));
    location.push_back(Position(3, 28));
    location.push_back(Position(3, 30));
    location.push_back(Position(3, 32));
    location.push_back(Position(3, 34));
    length = 6;
}
bool Snake::isCrashMySelf() {
    bool crash = false;
    int headRow = location[0].row;
    int headCol = location[0].col;

    for (int i=1; i<length; i++) {
        if (headRow == location[i].row && headCol == location[i].col) {
            return true;
        }
    }
    return crash;
}