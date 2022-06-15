#include <iostream>
#include "Snake.h"
using namespace std;

Snake::Snake(char dir) {
    direction = dir;
    location.push_back(Position(3, 34));
    location.push_back(Position(3, 36));
    location.push_back(Position(3, 38));
    location.push_back(Position(3, 40));
    location.push_back(Position(3, 42));
    location.push_back(Position(3, 44));
}
void Snake::initSnake(char dir) {
    direction = dir;
    location.clear();
    location.push_back(Position(3, 34));
    location.push_back(Position(3, 36));
    location.push_back(Position(3, 38));
    location.push_back(Position(3, 40));
    location.push_back(Position(3, 42));
    location.push_back(Position(3, 44));
    length = 6;
    maxLength = 6;
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