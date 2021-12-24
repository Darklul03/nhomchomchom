#pragma once
#include <SDL2/SDL.h>
#include "Snake.hpp"

class MainGame {
public:
    MainGame();
    ~MainGame();
    void run();

private:
    void initSystem();
    void draw();
    void changeDir();
    void initTexture();
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprites;
    int screenWidth, screenHeight;
    static const int FRAME_RATE = 1000/60;
    Snake snake;
    int counter;
};
