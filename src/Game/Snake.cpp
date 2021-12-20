#include "Snake.hpp"
#include <random>
Snake::Snake() {
    alive = true;
    segment.push_back({0,0});
    screenWidth = 1280;
    screenHeight = 768;
    generateFood();
    dir = direction::East;
}

void Snake::generateFood() {
    bool finish = false;
    int w = screenWidth/64;
    int h = screenHeight/64;
    std::mt19937 gen(time(0));
    do {
        Food.x = gen()%w;
        Food.y = gen()%h;
        finish = true;
        for (auto [x,y] : segment) {
            if (Food.x == x && Food.y == y) {
                finish = false;
                break;
            }
        if (Food.x < 0 || Food.y < 0 || Food.x >= w || Food.y >= h)
            finish = false;
        }
    } while (!finish);
}   

void Snake::Advance() {
    int dx[] = {0,1,-1,0,0};
    int dy[] = {0,0,0,1,-1};

    int w = screenWidth/64;
    int h = screenHeight/64;

    Node Head = segment.front();
    Head.x = (Head.x + dx[static_cast<int>(dir)]) % w;
    Head.y = (Head.y + dy[static_cast<int>(dir)]) % h;
    if (Head.x < 0) Head.x += w;
    if (Head.y < 0) Head.y += h;
    for (auto seg : segment)
        if (Head == seg) {
            alive = false;  
            break;
        }

    segment.push_front(Head);
    if (Head == Food)
        generateFood();
    else
        segment.pop_back();
}

void Snake::SetDirection(direction nextdir) {
    if (segment.size() == 1) {
        dir = nextdir;
        return;
    }
    switch (dir) {
    case direction::East:
        if (nextdir == direction::West) return;
        break;

    case direction::West:
        if (nextdir == direction::East) return;
        break;

    case direction::South:
        if (nextdir == direction::North) return;
        break;

    case direction::North:
        if (nextdir == direction::South) return;
        break;
    }
    dir = nextdir;
}
