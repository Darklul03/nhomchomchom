#include "MainGame.hpp"

MainGame::MainGame() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1280, screenHeight = 768;
    counter = 0;
    state = 0;
}

void MainGame::cleanup() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font48);
    TTF_Quit();
    SDL_Quit();
};

SDL_Texture* MainGame::loadTexture(const char* filePath) {
    SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

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
    sprites = loadTexture("resource/image/snake.bmp");
    endOverlay = loadTexture("resource/image/end.png");
    font48 = TTF_OpenFont("resource/font/font.ttf", 48);
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

void MainGame::run() {
    initSystem();
    initResource();
    unsigned int before, after;

    while (snake.alive) {
        before = SDL_GetTicks();
        if (counter == 0) {
            snake.Advance();
            // if (!snake.alive)
            //     return;
            draw();
        }
        changeDir();

        after = SDL_GetTicks();
        unsigned int time = after - before;
        if (FRAME_RATE > time)
            SDL_Delay(FRAME_RATE-time);

        counter = (counter+1)%10;
    }

    renderTexture(0,0,endOverlay);
    renderFontCenter(0, 0, "YOU DIED", font48, white);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
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

void MainGame::renderScore() {
    Uint8 r,g,b,a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_Rect rect = {0, screenHeight-TILESIZE, screenWidth, TILESIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderFillRect(renderer, &rect);

    int score = (snake.segment.size() - 2)*100;
    std::string s = "Score: " + std::to_string(score);
    renderFontCenter(0, screenHeight/2-TILESIZE/2, s.c_str(), font48, white);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void MainGame::draw() {
    if (snake.alive == false) return;
    SDL_SetRenderDrawColor(renderer, 0xf7, 0xe6, 0x97, 0x80); // background color
    SDL_RenderClear(renderer);

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
