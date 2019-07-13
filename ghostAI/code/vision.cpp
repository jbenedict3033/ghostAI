#include <iostream>

#include "vision.hpp"

enum {COLOR_NO_ALPHA = 255, COLOR_DEFAULT = 55, COLOR_WALL = 0};

Vision::Vision(int numLines, int x, int y) {
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)) {
        std::cout << "sdl not created\n"; SDL_Quit();
    }

	screen = SDL_CreateWindow("Ghost Vision", x, y,
        SDL::SCREEN_WIDTH, SDL::SCREEN_HEIGHT, SDL_WINDOW_HIDDEN);
    if(!screen) {
	    std::cout << "failed to create screen\n"; SDL_Quit();
    }
    renderer = SDL_CreateRenderer(screen, -1, 0);
    if(!renderer) {
        std::cout << "failed to create renderer\n"; SDL_Quit();
    }
    SDL_SetRenderDrawColor(renderer, 37, 160, 30, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    map_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET, SDL::SCREEN_WIDTH,
        SDL::SCREEN_HEIGHT);
	lineCount = numLines;
	lineLength = LevelReader::lineLength;
	tile.w = tile.h = TILE_SIZE;
}

SDL_Renderer* Vision::getRenderer() { return renderer; }

void Vision::drawVision() {
    for (int height = 0; height <= lineCount; height++) {
        for (int width = 0; width < lineLength; width++) {
            tile.x = width * TILE_SIZE;
            tile.y = height * TILE_SIZE;
            if (Ghost::wallArray[height][width] == -1)
                SDL_SetRenderDrawColor(renderer,
                COLOR_WALL, COLOR_WALL, COLOR_WALL, COLOR_NO_ALPHA);
            else SDL_SetRenderDrawColor(renderer,
                COLOR_DEFAULT, COLOR_DEFAULT, COLOR_DEFAULT,
                COLOR_NO_ALPHA);
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    SDL_RenderPresent(renderer);
}

void Vision::updateVision() {
    for (int height = 0; height <= lineCount; height++) {
        for (int width = 0; width < lineLength; width++) {
            tile.x = width * TILE_SIZE;
            tile.y = height * TILE_SIZE;
            if ((Ghost::wallArray[height][width] < .5) &&
            (Ghost::wallArray[height][width] > 0)) {
                Ghost::wallArray[height][width] += .0025;
                SDL_SetRenderDrawColor(renderer,
                255 - Ghost::wallArray[height][width] * 400,
                COLOR_DEFAULT, COLOR_DEFAULT, COLOR_NO_ALPHA);
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void Vision::showVision() {
    return;
}

void Vision::visionLoop() {
    renderPresent();
}

void Vision::renderPresent() {
    SDL_RenderPresent(renderer);
}
