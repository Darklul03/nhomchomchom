#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Snake.hpp"

const SDL_Color white = {255,255,255};
const SDL_Color black = {0,0,0};

class MainGame {
public:
    MainGame();
    void run();
    void cleanup();

private:
    void initSystem();
    SDL_Texture* loadTexture(const char* filePath);
    void initResource();
    void renderScore();
    void draw();
    void changeDir();
    SDL_Window *window;
    SDL_Renderer *renderer;
    int screenWidth, screenHeight;
    static const int FRAME_RATE = 1000/60;
    Snake snake;
    int counter;
    int state;

    void renderTexture(int x, int y, SDL_Texture* tex);
    SDL_Texture *sprites;
    SDL_Texture *endOverlay;

    void renderFontCenter(int x, int y, const char* text, TTF_Font* font, SDL_Color textcolor);
    TTF_Font* font48;
};
