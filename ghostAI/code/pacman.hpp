#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "sdl.hpp"
#include "ghost.hpp"

class Level;

class Pacman
{
public:
	Pacman(int X, int Y, std::string path, SDL_Texture* ScreenCache,
              SDL_Texture* EmptyTexture, SDL_Renderer* Renderer);

	static SDL_Rect* dots;
	static SDL_Rect* bigDots;
	static SDL_Rect* teleporters;
	static int numDots;
	static int numBigDots;
	static int numTeleporters;
	static int pacTime;

	static void updateDots(SDL_Rect* Dots, int numdots);
	static void updateBigDots(SDL_Rect* BigDots, int numbigdots);
	static void updateTeleporters(SDL_Rect* Teleporters, int numteleporters);

	int getPoints();
	void loadImage(std::string path);
	void handleEvents(SDL_Event ev);
	void move();
	void show();
	void drawImage(SDL_Rect clip);
	void collision();
	void setClips();
	int getPosX();
	int getPosY();
	int pacIsPowerful();
	bool pacAtePower();
	void atePowerDone();

private:
    SDL_Renderer* renderer;
	SDL_Texture* screenCache;
	SDL_Texture* emptyTile;
	SDL_Surface* pacman;
	SDL_Texture* pacTex;
	SDL_Rect clips[4][3];
	SDL_Rect pos;
	int frame;
	int status;
	int points;
	int pacSpeedHandicap;
	int powerTime;
	int powerLength;
	int pacHasPower;
	bool atePower;
	int keysDown[4];
	int movements[4];
	int keysCount;
	int maxIndex;
    int maxValue;
    int prevMaxIndex;
};

#endif                                              //PACMAN_HPP
