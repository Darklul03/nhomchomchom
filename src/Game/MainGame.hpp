#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Snake.hpp"

const SDL_Color white = {255,255,255};
const SDL_Color black = {0,0,0};
enum State {Title, Game, Pause, End};

class MainGame {
public:
    MainGame();
    void run();
    void titlescreen();
    void update();
    void pause();
    void end();
    void cleanup();

private:
    void initSystem();
    void initResource();
    SDL_Texture* loadTexture(const char* filePath);
    void renderScore();
    void draw();
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    static const int FRAME_RATE = 1000/60;
    Snake snake;
    int counter;
    State state;
    bool running;
    bool paused;
    int highScore;

    void renderTexture(int x, int y, SDL_Texture* tex);
    SDL_Texture* sprites;
    SDL_Texture* endOverlay;
    SDL_Texture* bg;
    SDL_Texture* click2start;
    SDL_Texture* continueButton;
    SDL_Texture* retryButton;
    SDL_Texture* quitButton;

    void renderFontCenter(int x, int y, const char* text, TTF_Font* font, SDL_Color textcolor);
    TTF_Font* font48;
    TTF_Font* font90;
};
