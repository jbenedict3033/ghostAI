#include <iostream> // used for error messages

#include "level.hpp"
#include "ghost.hpp"

/// static variables
int Ghost::penTime = 5000;
SDL_Rect Ghost::insideDoor[2] = { 0 };
SDL_Rect Ghost::outsideDoor[2] = { 0 };
std::vector<std::vector<float> > Ghost::wallArray(1);

/// constructor
Ghost::Ghost(int X, int Y, std::string path, SDL_Renderer* Renderer,
             int Order) {
    renderer = Renderer;
    order = Order;
    pos.x = spawnPos.x = X;
    pos.y = spawnPos.y = Y;
    pos.w = pos.h = TILE_SIZE;
    setClips();
    actionStatus = GET_TO_DOOR;
    loadImage(path);
    releaseTime = SDL_GetTicks() + penTime * order;
    toggleLength = 100;
    toggleTime = SDL_GetTicks() + toggleLength;
    toggle = false;
    pacPowerFlag = false;
    for (int MOVEMENT = RIGHT; MOVEMENT < MOVE_NUM; MOVEMENT++)
        movements[MOVEMENT] = .5;
    for (int ACTION = GET_TO_DOOR; ACTION < ACT_NUM; ACTION++)
        actions[ACTION] = .5;
    memory = std::vector<SDL_Rect>(MEMORY_SIZE);
    oldMemory = 0;
}

/// file retrieval function
void Ghost::loadImage(std::string path) {
	ghost = NULL;
    ghost = SDL_LoadBMP(path.c_str());
    if (!ghost) { std::cout << "no ghost image loaded\n"; exit(1); }
    tex = SDL_CreateTextureFromSurface(renderer, ghost);
    if (!tex) { std::cout << "no ghost texture created\n"; exit(1); }
}

/// get functions
SDL_Surface* Ghost::getGhost() { return ghost; }
SDL_Rect Ghost::getPos() { return pos; }
SDL_Rect Ghost::getSpawnPos() { return spawnPos; }
int Ghost::getActionStatus() { return actionStatus; }
int Ghost::getReleaseTime() { return releaseTime; }



/** central loop ***************************************************/

bool Ghost::ghostLoop(int x, int y, unsigned int ghostTime,
    bool& updated, int pacPower, bool atePower) {
    bool killedPacman = 0;
    /*if (actionStatus == AVOID_PACMAN) updated = (SDL_GetTicks() >= (ghostTime + 100));
    else*/ updated = (SDL_GetTicks() >= ghostTime);
    if (updated) {
        resetActions();
        resetMovements();
        pacPos.x = x;
        pacPos.y = y;
        if (atePower && actionStatus == KILL_PACMAN)
            actionStatus = AVOID_PACMAN;
        if (pacPower > 0) pacPowerFlag = true;
        else pacPowerFlag = false;
        if (pacCollision()) killedPacman = 1;
        selectAction();
        doAction();
        if (pacCollision()) killedPacman = 1;
    }
    else {
        pacPos.x = x;
        pacPos.y = y;
        if (pacCollision()) killedPacman = 1;
    }
    if (pacPower == 2 && actionStatus == AVOID_PACMAN) show(SCARED);
    else if (pacPower == 1 && actionStatus == AVOID_PACMAN)
        toggleBehavior();
    else show(CALM);
    return killedPacman;
}
/** end central loop ***********************************************/



/// action functions
void Ghost::resetActions() {
    for (int ACTION = GET_TO_DOOR; ACTION < ACT_NUM; ACTION++)
        actions[ACTION] = .5;
}

void Ghost::selectAction() {
    if (actionStatus == GET_TO_DOOR) actions[GET_TO_DOOR] = 1;
    if (actionStatus == GET_OUT_DOOR) actions[GET_OUT_DOOR] = 1;
    if (actionStatus == AVOID_PACMAN) actions[AVOID_PACMAN] = 1;
    if (actionStatus == KILL_PACMAN) actions[KILL_PACMAN] = 1;
    chosenAction = std::distance(actions,
        std::max_element(actions, actions + ACT_NUM));
}

void Ghost::doAction() {
    switch (chosenAction) {

        case GET_TO_DOOR:
            if (distance(pos.x, pos.y, insideDoor[0].x,
                insideDoor[0].y) < distance(pos.x, pos.y,
                insideDoor[1].x, insideDoor[1].y)) {
                targetPos.x = insideDoor[0].x;
                targetPos.y = insideDoor[0].y;
            }
            else {
                targetPos.x = insideDoor[1].x;
                targetPos.y = insideDoor[1].y;
            }
            if (SDL_GetTicks() >= releaseTime &&
                distance(pos.x, pos.y, targetPos.x, targetPos.y)
                == TILE_SIZE) {
                pos.x = targetPos.x;
                pos.y = targetPos.y;
                actionStatus = GET_OUT_DOOR;
            }
            else {
                chase(targetPos.x, targetPos.y);
            }
            break;



        case GET_OUT_DOOR:
            if (distance(pos.x, pos.y, outsideDoor[0].x,
                outsideDoor[0].y) < distance(pos.x, pos.y,
                outsideDoor[1].x, outsideDoor[1].y)) {
                targetPos.x = outsideDoor[0].x;
                targetPos.y = outsideDoor[0].y;
            }
            else {
                targetPos.x = outsideDoor[1].x;
                targetPos.y = outsideDoor[1].y;
            }
            if (pos.x == targetPos.x && pos.y == targetPos.y)
                actionStatus = KILL_PACMAN;
            else
                chase(targetPos.x, targetPos.y);
            break;



        case KILL_PACMAN:
            chase(pacPos.x, pacPos.y);
            break;



        case AVOID_PACMAN:
            chase(spawnPos.x, spawnPos.y);
            if (!pacPowerFlag) actionStatus = KILL_PACMAN;
            break;
    }
}

bool Ghost::pacCollision() {
    if ((pos.x == pacPos.x) && (pos.y == pacPos.y)) {
        if (actionStatus == AVOID_PACMAN) {
            respawn(); return false;
        }
        else return true;
    }
    else return false;
}

void Ghost::respawn() {
    pos.x = spawnPos.x;
    pos.y = spawnPos.y;
    actionStatus = GET_TO_DOOR;
    releaseTime = SDL_GetTicks() + penTime;
}

/// movement functions
void Ghost::resetMovements() {
    for (int MOVEMENT = RIGHT; MOVEMENT < MOVE_NUM; MOVEMENT++)
        movements[MOVEMENT] = .5;
}

void Ghost::chase(int squareX, int squareY) {

    int xDirection, yDirection;
    if (squareX - pos.x > 0) xDirection = RIGHT;
    if (squareX - pos.x < 0) xDirection = LEFT;
    if (squareX - pos.x == 0) xDirection = std::rand() % 2;
    if (squareY - pos.y > 0) yDirection = DOWN;
    if (squareY - pos.y < 0) yDirection = UP;
    if (squareY - pos.y == 0) yDirection = std::rand() % 2 + 2;
    movements[xDirection] += .1;
    movements[yDirection] += .1;
    std::abs(squareX - pos.x) > std::abs(squareY - pos.y) ?
        movements[xDirection] += -.05 +
            (8.0 / distance(pos.x, pos.y, squareX, squareY)) :
        movements[yDirection] += -.05 +
            (8.0 / distance(pos.x, pos.y, squareX, squareY));
    if (pos.y == Pacman::teleporters[0].y) {
        if (std::abs(Pacman::teleporters[0].x - pos.x) >
            std::abs(Pacman::teleporters[1].x - pos.x))
            movements[RIGHT] += .01;
        if (std::abs(Pacman::teleporters[0].x - pos.x) <
            std::abs(Pacman::teleporters[1].x - pos.x))
            movements[LEFT] += .01;
    }

    checkMap();

    chosenMovement = std::distance(movements,
        std::max_element(movements, movements + MOVE_NUM));
    if (portFlag == 0) {
        chosenMovement = RIGHT;
        portFlag = -1;
    }
    if (portFlag == 1) {
        chosenMovement = LEFT;
        portFlag = -1;
    }

    switch (chosenMovement) {
        case RIGHT:
            pos.x += TILE_SIZE;
            break;
        case LEFT:
            pos.x -= TILE_SIZE;
            break;
        case DOWN:
            pos.y += TILE_SIZE;
            break;
        case UP:
            pos.y -= TILE_SIZE;
            break;
    }
    for(int i = 0; i < Pacman::numTeleporters; i++) {
		SDL_Rect temp = Pacman::teleporters[i];
		if(temp.x == pos.x && temp.y == pos.y) {
            if(i == 0) {
				pos.x = Pacman::teleporters[1].x;
				pos.y = Pacman::teleporters[1].y;
				portFlag = 1;
                return;
			}
			else if (i == 1) {
				pos.x = Pacman::teleporters[0].x;
				pos.y = Pacman::teleporters[0].y;
				portFlag = 0;
			}
		}
	}
}

void Ghost::checkMap() {
    movements[RIGHT] *= wallArray[pos.y / TILE_SIZE]
        [(pos.x / TILE_SIZE) + 1];
    movements[LEFT] *= wallArray[pos.y / TILE_SIZE]
        [(pos.x / TILE_SIZE) - 1];
    movements[DOWN] *= wallArray[(pos.y / TILE_SIZE) + 1]
        [pos.x / TILE_SIZE];
    movements[UP] *= wallArray[(pos.y / TILE_SIZE) - 1]
        [pos.x / TILE_SIZE];
}

void Ghost::checkCollisions() {
    if (wallArray[pos.y / TILE_SIZE][(pos.x / TILE_SIZE) + 1] < 0)
        movements[RIGHT] = 0;
    if (wallArray[pos.y / TILE_SIZE][(pos.x / TILE_SIZE) - 1] < 0)
        movements[LEFT] = 0;
    if (wallArray[(pos.y / TILE_SIZE) + 1][pos.x / TILE_SIZE] < 0)
        movements[DOWN] = 0;
    if (wallArray[(pos.y / TILE_SIZE) - 1][pos.x / TILE_SIZE] < 0)
        movements[UP] = 0;
}

/// helper functions
double Ghost::distance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

/// behavior functions
void Ghost::setClips() {
    for (int BEHAVIOR = 0; BEHAVIOR <= SCARED; BEHAVIOR++) {
        for (int GHOST = BLINKY; GHOST <= CLYDE; GHOST++) {
            clips[BEHAVIOR][GHOST].x = TILE_SIZE * GHOST;
            clips[BEHAVIOR][GHOST].y = TILE_SIZE * BEHAVIOR;
            clips[BEHAVIOR][GHOST].w = TILE_SIZE;
            clips[BEHAVIOR][GHOST].h = TILE_SIZE;
        }
    }
}

void Ghost::show(int behavior) {
    SDL_RenderCopy(renderer, tex, &(clips[behavior][order]), &pos);
}

void Ghost::toggleBehavior() {
    if (SDL_GetTicks() >= toggleTime) {
        toggle = !toggle;
        toggleTime = SDL_GetTicks() + toggleLength;
    }
    show(toggle);
}
