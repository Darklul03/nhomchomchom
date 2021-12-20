#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "Snake.hpp"

class MainGame {
public:
    MainGame();
    ~MainGame();
    void run();

private:
    void initSystems();
    void draw();
    void changeDir();
    SDL_Window *window;
    SDL_Renderer *renderer;
    int screenWidth, screenHeight;
    static const int FRAME_RATE = 1000/60;
    Snake snake;
    int counter;
};
