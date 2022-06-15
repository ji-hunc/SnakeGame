#include <iostream>
#include "Position.h"
#include <vector>
using namespace std;

class Snake {
private:
    int locationRow;
    int locationCol;
    int length = 6;
    char direction;
    // vector<Position> location;
public:
    Snake(char dir);
    vector<Position> location;
    void initSnake(char dir);
    bool isCrashMySelf();
    void setDirection(char dir) {
        direction = dir;
    }
    char getDirection() {
        return direction;
    }
    int getLength() {
        return length;
    }
    void move_up() {
        locationRow--;
    }
    void move_down() {
        locationRow++;
    }
    void move_right() {
        locationCol++;
    }
    void move_left() {
        locationCol--;
    }
    void eatApple(int appleR, int appleC) {
        length++;
        location.insert(location.end(), Position(location[location.size()-1].row, location[location.size()-1].col));
        
    }
    void eatPoison(int poisonR, int poisonC) {
        location.erase(location.end()-1);
        length--;
    }
};