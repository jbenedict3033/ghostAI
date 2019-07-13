#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <map>
#include <vector>

#include "sdl.hpp"
#include "pacman.hpp"
#include "levelReader.hpp"
#include "ghost.hpp"
#include "vision.hpp"

class Vision;
class Pacman;
class Ghost;

enum { RIGHT = 0,   LEFT = 1,   DOWN = 2,   UP = 3,     MOVE_NUM = 4,
    TILE_SIZE = 16 };

class Level {
public:
	Level(int numLines, std::map<int, std::string> fileText,
       SDL Sdl);
    static unsigned int ghostTime;
    static int ghostOrder;
	void addToWalls(int x, int y);
	void addToDots(int x, int y);
	void addToBigDots(int x, int y);
	void addToTeleporters(int x, int y);
	void addToOuterDoors(int x, int y);
	void addToInnerDoors(int x, int y);
	int getWallNum();
	int getDotNum();
	int getBigDotNum();
	int getTeleporterNum();
	void drawLevel();
	void handleLine(std::string Char, int y);
	Pacman* getPacman();
	SDL_Rect* getWalls();
	Ghost** getGhostGang();
	bool winningScore();
	bool losingScore();
	void setWallClips();
	void levelLoop();
	void ghostGangLoop();
	void createVision(int x, int y);
	void updateMap(Ghost* ghost);
private:
	Pacman* pacman;
	std::map<int, std::string> text;
	SDL sdl;
	SDL_Rect dots[1000];
	SDL_Rect bigDots[4];
	SDL_Rect teleporters[2];
	SDL_Rect wallClips[18];
	int dotNum;
	int bigDotNum;
	int teleporterNum;
	int outerDoorNum;
	int innerDoorNum;
	int lineCount;
	int lineLength;
	Ghost* ghostGang[4];
	int ghostSpeedHandicap;
	bool gameLost;
    Vision* vision;
};

#endif                                              //LEVEL_HPP
