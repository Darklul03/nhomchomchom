#pragma once
#include <deque>
#include <iostream>
#include <SDL2/SDL.h>
#define TILESIZE 64

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
    std::deque<direction> segmentDir;
    Node Food;

private:
    int screenHeight;
    int screenWidth;
    void GenerateFood();
    bool changed;
};
