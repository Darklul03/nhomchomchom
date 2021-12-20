#include "MainGame.hpp"

MainGame::MainGame() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1280, screenHeight = 768;
    counter = 0;
}

MainGame::~MainGame() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
};

void MainGame::initSystems() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void MainGame::run() {
    initSystems();
    unsigned int before, after;

    while (snake.alive) {
        before = SDL_GetTicks();
        if (counter%10 == 0)
            snake.Advance();
        changeDir();
        draw();

        after = SDL_GetTicks();
        unsigned int time = after - before;
        if (FRAME_RATE > time)
            SDL_Delay(FRAME_RATE-time);
        counter++;
    }
}

void MainGame::changeDir() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            snake.alive = false;

        if (event.type = SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                snake.SetDirection(direction::East);
                break;

            case SDLK_LEFT:
                snake.SetDirection(direction::West);
                break;

            case SDLK_DOWN:
                snake.SetDirection(direction::South);
                break;

            case SDLK_UP:
                snake.SetDirection(direction::North);
                break;

            case SDLK_ESCAPE:
                snake.alive = false;

            default:
                break;
            }
        }
    }
}

void MainGame::draw() {
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0xff, 0xff);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
    for (auto seg : snake.segment) {
        SDL_Rect r = {seg.x*64, seg.y*64, 64, 64};
        SDL_RenderFillRect(renderer, &r);
        if (seg == snake.segment.front())
            SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0xff);
    }

    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
    SDL_Rect r = {snake.Food.x*64, snake.Food.y*64, 64, 64};
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
}
