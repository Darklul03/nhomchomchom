#pragma once
#include <deque>
#include <iostream>
#include <SDL2/SDL.h>

enum class direction {Stop, East, West, South, North};
struct Node {
    int x;
    int y;
    bool operator == (Node a) {
        return (a.x == x) && (a.y == y);
    }
};

class Snake {
public:
    Snake();
    void Advance();
    void SetDirection(direction nextdir);
    bool alive;
    std::deque<Node> segment;
    direction dir;
    Node Food;

private:
    int screenHeight;
    int screenWidth;
    void generateFood();
};
