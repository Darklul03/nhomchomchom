#include "MainGame.hpp"

MainGame::MainGame() {
    window = nullptr;
    renderer = nullptr;
    counter = 0;
    highScore = 0;
    state = Title;
    running = true;
    paused = false;
}

void MainGame::cleanup() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font48);
    TTF_CloseFont(font90);
    TTF_Quit();
    SDL_Quit();
};

SDL_Texture* MainGame::loadTexture(const char* filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);

    return texture;
}

void MainGame::renderTexture(int x, int y, SDL_Texture* tex) {
    SDL_Rect src; 
    src.x = 0;
    src.y = 0;
    src.w;
    src.h;

    SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, tex, &src, &dst);
}

void MainGame::initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void MainGame::initResource() {
    font48 = TTF_OpenFont("resource/font/font.ttf", 48);
    font90 = TTF_OpenFont("resource/font/font.ttf", 90);

    sprites = loadTexture("resource/image/snake.bmp");
    endOverlay = loadTexture("resource/image/end.png");
    bg = loadTexture("resource/image/bg.png");
    click2start = loadTexture("resource/image/click2start.png");
    continueButton = loadTexture("resource/image/pause-continue.png");
    retryButton = loadTexture("resource/image/pause-retry.png");
    quitButton = loadTexture("resource/image/pause-back.png");
}

void MainGame::renderFontCenter(int x, int y, const char* text, TTF_Font* font, SDL_Color textcolor) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, textcolor);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src = {0, 0, surfaceMessage->w, surfaceMessage->h};
    SDL_Rect dest = {screenWidth/2 - src.w/2 + x, screenHeight/2 - src.h/2 + y, src.w, src.h};

    SDL_RenderCopy(renderer, message, &src, &dest);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void MainGame::titlescreen() {
    SDL_RenderClear(renderer);
    renderTexture(0, 0, bg);
    renderTexture(0, 0, click2start);
    renderFontCenter(0, -120, "SNAKE", font90, black);
    renderFontCenter(0, 384 - 16*13 + 4 - 3, "LEFT CLICK TO START", font48, black);
    renderFontCenter(0, 384 - 16*13 + 4, "LEFT CLICK TO START", font48, white);

    if (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                state = Game;
            }
            break;
        }
    }
    SDL_RenderPresent(renderer);
}

void MainGame::pause() {
    renderTexture(441, 160, continueButton);
    renderTexture(441, 320, retryButton);
    renderTexture(441, 480, quitButton);
    SDL_RenderPresent(renderer);

    if (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                if ((x > 441) && (x < 441+398) && (y > 160) && (y < 160+116)) {
                    paused = !paused;
                    state = Game;
                }
                if ((x > 441) && (x < 441+398) && (y > 320) && (y < 320+116)) {
                    snake.newSnake();
                    paused = !paused;
                    state = Game;
                }
                if ((x > 441) && (x < 441+398) && (y > 480) && (y < 480+116))
                    running = false;
            }
            break;

        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (paused)
                    state = Game;
                paused = !paused;
            }
            break;
        }
    }
}

void MainGame::end() {
    if (snake.segment.size() == (screenWidth/TILESIZE)*(screenHeight/TILESIZE-1))
        renderFontCenter(0, -240, "YOU WIN", font48, white);
    else
        renderFontCenter(0, -240, "YOU DIED", font48, white);

    int score = (snake.segment.size()-2)*100;
    if (score > highScore) {
        std::string s = std::to_string(score);
        s = "NEW HIGH SCORE: " + s;
        renderFontCenter(0, -160, s.c_str(), font48, white);
    }
    else {
        std::string s = std::to_string(score);
        std::string hs = "HIGH SCORE: " + std::to_string(highScore);
        s = "SCORE: " + s;
        renderFontCenter(0, -120, hs.c_str(), font48, white);
        renderFontCenter(0, -160, s.c_str(), font48, white);
    }

    renderTexture(441, 320, retryButton);
    renderTexture(441, 480, quitButton);

    if (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                if ((x > 441) && (x < 441+398) && (y > 320) && (y < 320+116)) {
                    highScore = score;
                    snake.newSnake();
                    state = Game;
                }
                if ((x > 441) && (x < 441+398) && (y > 480) && (y < 480+116))
                    running = false;
            }
            break;
        }
    }

    SDL_RenderPresent(renderer);
}

void MainGame::update() {
    if (counter == 0) {
        snake.Advance();
        draw();
        if (snake.alive == false) {
            renderTexture(0, 0, endOverlay);
            state = End;
            return;
        }
    }

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }

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
                renderTexture(0, 0, endOverlay);
                state = Pause;
                break;
            }
        }
    }

    counter = (counter+1)%10;
}

void MainGame::run() {
    initSystem();
    initResource();

    while (running) {
        unsigned int time;
        time = SDL_GetTicks();

        if (state == Title)
            titlescreen();
        if (state == Game)
            update();
        if (state == Pause)
            pause();
        if (state == End)
            end();

        time = SDL_GetTicks() - time;
        if (FRAME_RATE > time)
            SDL_Delay(FRAME_RATE-time);
    }
}

void MainGame::renderScore() {
    SDL_Rect rect = {0, screenHeight-TILESIZE, screenWidth, TILESIZE};
    SDL_RenderFillRect(renderer, &rect);

    int score = (snake.segment.size() - 2)*100;
    std::string s = "SCORE: " + std::to_string(score);
    renderFontCenter(0, screenHeight/2-TILESIZE/2, s.c_str(), font48, white);
}

void MainGame::draw() {
    if (snake.alive == false) return;
    SDL_RenderClear(renderer);
    renderTexture(0, 0, bg);

    for (int i = 0; i < snake.segment.size(); i++) {
        Node seg = snake.segment[i];
        direction currentDir = snake.segmentDir[i];
        SDL_Rect dest = {seg.x*TILESIZE, seg.y*TILESIZE, TILESIZE, TILESIZE};
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

    SDL_Rect dest = {snake.Food.x*TILESIZE, snake.Food.y*TILESIZE, TILESIZE, TILESIZE};
    SDL_Rect src = {0,64*3,64,64};
    SDL_RenderCopy(renderer,sprites,&src,&dest);
    renderScore();
    SDL_RenderPresent(renderer);
}
