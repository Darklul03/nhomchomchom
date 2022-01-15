#pragma once
#include <deque>
#define TILESIZE 64
#define screenWidth 1280
#define screenHeight 768

enum class direction {East, West, South, North};
struct Node {
    int x;
    int y;

    bool operator == (Node a) {
        return (this->x == a.x) && (this->y == a.y);
    }
};

class Snake {
public:
    Snake();
    void newSnake();
    void Advance();
    void SetDirection(direction nextdir);
    bool alive;
    std::deque<Node> segment;
    std::deque<direction> segmentDir;
    Node Food;

private:
    int w;
    int h;
    void GenerateFood();
    bool changed;
};
