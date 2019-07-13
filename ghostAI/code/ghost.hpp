#ifndef GHOST_HPP
#define GHOST_HPP

#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cmath>

enum {
    BLINKY = 0,
    PINKY = 1,
    INKY = 2,
    CLYDE = 3,
    CALM = 0,
    SCARED = 1,
	GET_TO_DOOR = 0,
	GET_OUT_DOOR = 1,
	AVOID_PACMAN = 2,
	KILL_PACMAN = 3,
	ACT_NUM = 4,
	MEMORY_SIZE = 10
};

class Ghost {
public:
    Ghost(int X, int Y, std::string path, SDL_Renderer* Renderer,
          int order);
    static int penTime;
    static SDL_Rect insideDoor[2];
    static SDL_Rect outsideDoor[2];
    static std::vector<std::vector<float> > wallArray;
    void loadImage(std::string path);
    void resetActions();
    void resetMovements();
    void selectAction();
    void doAction();
    void chase(int squareX, int squareY);
    void checkMap();
    void checkCollisions();
    void show(int behavior);
    void toggleBehavior();
    void respawn();
    void setClips();
    SDL_Surface* getGhost();
    SDL_Rect getPos();
    SDL_Rect getSpawnPos();
    int getActionStatus();
    int getReleaseTime();
    bool ghostLoop(int x, int y, unsigned int ghostTime, bool& updated,
                   int pacPower, bool atePower);
    bool pacCollision();
    double distance(int x1, int y1, int x2, int y2);
private:
    SDL_Renderer* renderer;
    SDL_Surface* ghost;
    SDL_Texture* tex;
    SDL_Rect pos;
    SDL_Rect spawnPos;
    SDL_Rect pacPos;
    SDL_Rect targetPos;
    SDL_Rect clips[2][4];
    int chosenAction;
    int chosenMovement;
    int actionStatus;
    unsigned int releaseTime;
    unsigned int toggleTime;
    int order;
    bool pacPowerFlag;
    bool toggle;
    int toggleLength;
    double movements[4];
    double actions[4];
    std::vector<SDL_Rect> memory;
    int oldMemory;
    int portFlag;
};

#endif                                              //GHOST_HPP
