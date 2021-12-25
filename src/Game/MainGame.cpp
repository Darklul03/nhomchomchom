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

void MainGame::initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void MainGame::initTexture() {
    SDL_Surface *surface = SDL_LoadBMP("snake.bmp");
    sprites = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
}

void MainGame::run() {
    initSystem();
    initTexture();
    unsigned int before, after;

    while (snake.alive) {
        before = SDL_GetTicks();
        if (counter == 0) {
            snake.Advance();
            draw();
        }
        changeDir();

        after = SDL_GetTicks();
        unsigned int time = after - before;
        if (FRAME_RATE > time)
            SDL_Delay(FRAME_RATE-time);

        counter = (counter+1)%10;
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
    SDL_SetRenderDrawColor(renderer, 0xf7, 0xe6, 0x97, 0x80); // background color
    SDL_RenderClear(renderer);

    for (int i = 0; i < snake.segment.size(); i++) {
        Node seg = snake.segment[i];
        direction currentDir = snake.segmentDir[i];
        SDL_Rect dest = {seg.x*64, seg.y*64, 64, 64};
        SDL_Rect src = {0,0,64,64};
        if (seg == snake.segment.front()) {
            if (currentDir == direction::East)
                src.x = 4, src.y = 0;
            if (currentDir == direction::North)
                src.x = 3, src.y = 0;
            if (currentDir == direction::West)
                src.x = 3, src.y = 1;
            if (currentDir == direction::South)
                src.x = 4, src.y = 1;
        }
        else if (seg == snake.segment.back()) {
            if (currentDir == direction::East)
                src.x = 4, src.y = 2;
            if (currentDir == direction::North)
                src.x = 3, src.y = 2;
            if (currentDir == direction::West)
                src.x = 3, src.y = 3;
            if (currentDir == direction::South)
                src.x = 4, src.y = 3;
        }
        else {
            direction prevDir = snake.segmentDir[i+1];
            if (currentDir == prevDir) { // horizontal or vertical
                if (currentDir == direction::North || currentDir == direction::South)
                    src.x = 2, src.y = 1;
                else
                    src.x = 1, src.y = 0;
            }
            else {
                if ((currentDir == direction::East && prevDir == direction::North) || (currentDir == direction::South && prevDir == direction::West))
                    // Up right
                    src.x = 0, src.y = 0;
                if ((currentDir == direction::West && prevDir == direction::North) || (currentDir == direction::South && prevDir == direction::East))
                    // Up left
                    src.x = 2, src.y = 0;
                if ((currentDir == direction::East && prevDir == direction::South) || (currentDir == direction::North && prevDir == direction::West))
                    // Down right
                    src.x = 0, src.y = 1;
                if ((currentDir == direction::West && prevDir == direction::South) || (currentDir == direction::North && prevDir == direction::East))
                    // Down left
                    src.x = 2, src.y = 2;
            }
        }
        src.x *= 64;
        src.y *= 64;
        SDL_RenderCopy(renderer,sprites,&src,&dest);
    }

    SDL_Rect dest = {snake.Food.x*64, snake.Food.y*64, 64, 64};
    SDL_Rect src = {0,64*3,64,64};
    SDL_RenderCopy(renderer,sprites,&src,&dest);
    SDL_RenderPresent(renderer);
}
