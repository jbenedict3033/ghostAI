#include <iostream>

#include "sdl.hpp"
#include "levelReader.hpp"

int SDL::SCREEN_WIDTH = 0;
int SDL::SCREEN_HEIGHT = 0;

void SDL::initializeSDL(int x, int y) {
	SCREEN_WIDTH = LevelReader::lineLength * TILE_SIZE;
	SCREEN_HEIGHT = (LevelReader::lineCount + 1) * TILE_SIZE;
	if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)) {
        std::cout << "no sdl made\n"; SDL_Quit();
    }

	screen = SDL_CreateWindow("Pacman", x, y, SCREEN_WIDTH,
        SCREEN_HEIGHT, 0);
    if(!screen) { std::cout << "no window made\n"; SDL_Quit(); }
    renderer = SDL_CreateRenderer(screen, -1, 0);
    if(!renderer) { std::cout << "no renderer made\n"; SDL_Quit(); }
    SDL_SetRenderDrawColor(renderer, 37, 160, 30, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    lvl_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    empty_tex = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
        TILE_SIZE, TILE_SIZE);
    SDL_Surface* empty_surface = SDL_LoadBMP(
        "graphics\\empty.bmp");
    empty_tex = SDL_CreateTextureFromSurface(renderer, empty_surface);
}

void SDL::loadBMP(std::string file, int x, int y, SDL_Texture* tex,
    SDL_Rect clip) {
	SDL_Surface* pic = NULL;
    SDL_Rect dest;
    pic = SDL_LoadBMP(file.c_str());
    if(!pic) { std::cout << "no image loaded\n"; exit(1); }
    dest.x = x;
    dest.y = y;
    dest.w = clip.w;
    dest.h = clip.h;
    SDL_Texture* tex2 =
        SDL_CreateTextureFromSurface(renderer, pic);
    if (!tex2) { std::cout << "no texture created\n"; exit(1); }
    SDL_FreeSurface(pic);
    pic = NULL;
    SDL_SetRenderTarget(renderer, tex);
    SDL_RenderCopy(renderer, tex2,
                   &clip, &dest);
    SDL_SetRenderTarget(renderer, NULL);
}

SDL_Surface* SDL::getScreen() {
	return SDL_GetWindowSurface(screen);
}

SDL_Texture* SDL::getLevelTexture() {
    return lvl_tex;
}

SDL_Texture* SDL::getEmptyTexture() {
    return empty_tex;
}

SDL_Renderer* SDL::getRenderer() {
	return renderer;
}
