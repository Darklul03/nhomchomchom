#include "Snake.hpp"
#include <random>

Snake::Snake() {
    w = screenWidth/TILESIZE;
    h = screenHeight/TILESIZE-1;
    newSnake();
}

void Snake::newSnake() {
    alive = true;
    segment.clear();
    segmentDir.clear();
    segment.push_back({0,0});
    segment.push_back({0,1});
    segmentDir.push_back(direction::East);
    segmentDir.push_back(direction::East);
    changed = false;
    GenerateFood();
}

void Snake::GenerateFood() {
    bool finish = false;
    std::mt19937 gen(time(0));
    std::uniform_int_distribution randx(0,w-1);
    std::uniform_int_distribution randy(0,h-1);
    do {
        Food.x = randx(gen);
        Food.y = randy(gen);
        finish = true;
        for (Node i : segment) {
            if (Food == i) {
                finish = false;
                break;
            }
        }
    } while (!finish);
}

void Snake::Advance() {
    direction dir = segmentDir.front();
    int dx[] = {1,-1,0,0};
    int dy[] = {0,0,1,-1};

    Node Head = segment.front();
    Head.x = (Head.x + dx[static_cast<int>(dir)]) % w;
    Head.y = (Head.y + dy[static_cast<int>(dir)]) % h;
    if (Head.x < 0) Head.x += w;
    if (Head.y < 0) Head.y += h;
    for (auto seg : segment) {
        if (seg == segment.back())
            continue;
        if (Head == seg) {
            alive = false;
            break;
        }
    }

    segment.push_front(Head);
    segmentDir.push_front(dir);
    if (Head == Food)
        GenerateFood();
    else {
        segment.pop_back();
        segmentDir.pop_back();
    }

    changed = false;
}

void Snake::SetDirection(direction nextdir) {
    direction &dir = segmentDir.front();
    if (segment.size() == 1) {
        dir = nextdir;
        return;
    }
    if (changed) return;

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
    changed = true;
}
