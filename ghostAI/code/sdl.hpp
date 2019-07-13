#ifndef SDL_HPP
#define SDL_HPP

#include <string>
#include <SDL.h>

class SDL {
public:
	void initializeSDL(int x, int y);
	void loadBMP(std::string file, int x, int y, SDL_Texture* tex,
        SDL_Rect clip);
	SDL_Surface* getScreen();
	SDL_Renderer* getRenderer();
	SDL_Texture* getLevelTexture();
	SDL_Texture* getEmptyTexture();
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

private:
	SDL_Window* screen;
	SDL_Renderer* renderer;
	SDL_Texture* empty_tex;
	SDL_Texture* lvl_tex;
};

#endif                                              //SDL_HPP
