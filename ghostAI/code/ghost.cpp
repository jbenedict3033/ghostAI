/**=================================================================

                        ghostAI - ghost.cpp
                           JOEY BENEDICT
                      ARTIFICIAL INTELLIGENCE

This file implements a novel multi-agent search algorithm to the
ghost NPCs in the game, in addition to creating the ghost objects
themselves.

=================================================================**/



/**________________   pre-compiler directives   _________________**/

#include <iostream>

#include "level.hpp"
#include "ghost.hpp"



/**______________________   declarations   ______________________**/

/*                      no global variables                       */



/**______________________   definitions   _______________________**/

/*                      static data members                       */

int Ghost::penTime = 5000;
SDL_Rect Ghost::insideDoor[2] = { 0 };
SDL_Rect Ghost::outsideDoor[2] = { 0 };
std::vector<std::vector<float> > Ghost::wallArray(1);

/*                          constructor                           */

Ghost::Ghost(int X, int Y, std::string path,
    SDL_Renderer* Renderer, int Order) {

    renderer = Renderer;                            //initialize
    pos.x = spawnPos.x = X;                         //variables
    pos.y = spawnPos.y = Y;
    pos.w = pos.h = TILE_SIZE;

    setClips();                                     //load sprite
    loadImage(path);                                //images

    actionStatus = GET_TO_DOOR;                     //start ghost
                                                    //inside of pen

    order = Order;                                  //"order"
                                                    //determines
                                                    //the ghost's
                                                    //color and
                                                    //when it can
                                                    //leave the pen

    releaseTime = SDL_GetTicks() +                  //define pen
        penTime * order;                            //release time

    toggleLength = 100;                             //start ghost
    toggleTime = SDL_GetTicks() + toggleLength;     //with normal,
    toggle = false;                                 //not scared
    pacPowerFlag = false;                           //behavior

    for (int MOVEMENT = RIGHT; MOVEMENT <           //set utilities
        MOVE_NUM; MOVEMENT++)                       //to starting
        movements[MOVEMENT] = .5;                   //values
    for (int ACTION = GET_TO_DOOR; ACTION <
        ACT_NUM; ACTION++) actions[ACTION] = .5;

    //memory = std::vector<SDL_Rect>(MEMORY_SIZE);  //for future use
}

/*                       load ghost texture                       */

void Ghost::loadImage(std::string path) {
	ghost = NULL;
    ghost = SDL_LoadBMP(path.c_str());
    if (!ghost) {
        std::cout << "no ghost image loaded\n"; exit(1);
    }
    tex = SDL_CreateTextureFromSurface(renderer, ghost);
    if (!tex) {
        std::cout << "no ghost texture created\n"; exit(1);
    }
}

/*                        getter functions                        */

SDL_Surface* Ghost::getGhost() { return ghost; }
SDL_Rect Ghost::getPos() { return pos; }
SDL_Rect Ghost::getSpawnPos() { return spawnPos; }
int Ghost::getActionStatus() { return actionStatus; }
int Ghost::getReleaseTime() { return releaseTime; }



/**_________________   central loop function   __________________**/

bool Ghost::ghostLoop(int x, int y, unsigned int ghostTime,
    bool& updated, int pacPower, bool atePower) {

    bool killedPacman = 0;                          //Pac Man is
                                                    //still alive

    updated = (SDL_GetTicks() >= ghostTime);        //check if frame
                                                    //needs to be
                                                    //updated

/*                   if frame is being updated                    */

    if (updated) {

        resetActions();                             //reset
        resetMovements();                           //utilities

        pacPos.x = x;                               //update Pac
        pacPos.y = y;                               //Man's location

        if (atePower &&                             //check if
            actionStatus == KILL_PACMAN)            //Pac Man ate a
            actionStatus = AVOID_PACMAN;            //power pellet
        if (pacPower > 0) pacPowerFlag = true;
        else pacPowerFlag = false;

        if (pacCollision()) killedPacman = 1;       //check if Pac
        selectAction();                             //Man was killed
        doAction();
        if (pacCollision()) killedPacman = 1;
    }

/*                 if frame is not being updated                  */

    else {
        pacPos.x = x;                               //update Pac
        pacPos.y = y;                               //Man's location

        if (pacCollision()) killedPacman = 1;       //check if Pac
                                                    //man was killed
    }

    if (pacPower == 2 && actionStatus ==            //update ghost's
        AVOID_PACMAN) show(SCARED);                 //appearance
    else if (pacPower == 1 && actionStatus ==       //based on its
        AVOID_PACMAN) toggleBehavior();             //behavior
    else show(CALM);

    return killedPacman;                            //tell the main
                                                    //function
                                                    //whether or not
                                                    //Pac Man died
}

/**_______________   end central loop function   ________________**/



/*                     reset action utilities                     */

void Ghost::resetActions() {
    for (int ACTION = GET_TO_DOOR; ACTION <
        ACT_NUM; ACTION++) actions[ACTION] = .5;
}

/*                         select action                          */

void Ghost::selectAction() {
    if (actionStatus == GET_TO_DOOR) actions[GET_TO_DOOR] = 1;
    if (actionStatus == GET_OUT_DOOR) actions[GET_OUT_DOOR] = 1;
    if (actionStatus == AVOID_PACMAN) actions[AVOID_PACMAN] = 1;
    if (actionStatus == KILL_PACMAN) actions[KILL_PACMAN] = 1;
    chosenAction = std::distance(actions,
        std::max_element(actions, actions + ACT_NUM));
}

/*                        carry out action                        */

void Ghost::doAction() {

    switch (chosenAction) {                         //see which
                                                    //action was
                                                    //chosen
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        case GET_TO_DOOR:                           //action:
                                                    //ghost stays
                                                    //in pen

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

            if (SDL_GetTicks() >= releaseTime &&    //check if ghost
                distance(pos.x, pos.y,              //can leave pen
                targetPos.x, targetPos.y) ==        //yet
                TILE_SIZE) {
                pos.x = targetPos.x;
                pos.y = targetPos.y;
                actionStatus = GET_OUT_DOOR;
            }
            else {
                chase(targetPos.x, targetPos.y);
            }
            break;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        case GET_OUT_DOOR:                          //action:
                                                    //ghost leaves
                                                    //pen

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

            if (pos.x == targetPos.x &&             //check if ghost
                pos.y == targetPos.y)               //has left pen
                actionStatus = KILL_PACMAN;         //yet
            else
                chase(targetPos.x, targetPos.y);
            break;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        case KILL_PACMAN:                           //action:
                                                    //ghost chases
                                                    //Pac Man

            chase(pacPos.x, pacPos.y);
            break;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        case AVOID_PACMAN:                          //action:
                                                    //ghost flees
                                                    //from Pac Man

            chase(spawnPos.x, spawnPos.y);

            if (!pacPowerFlag)                      //check if ghost
                actionStatus = KILL_PACMAN;         //can stop
                                                    //fleeing yet
            break;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    }
}

/*             check if Pac Man collided with a ghost             */

bool Ghost::pacCollision() {
    if ((pos.x == pacPos.x) && (pos.y == pacPos.y)) {

        if (actionStatus == AVOID_PACMAN) {         //check if
            respawn(); return false;                //Pac Man was
        }                                           //vulnerable
        else return true;                           //during the
    }                                               //collision

    else return false;
}

/*                        regenerate ghost                        */

void Ghost::respawn() {
    pos.x = spawnPos.x;
    pos.y = spawnPos.y;

    actionStatus = GET_TO_DOOR;                     //reset action
    releaseTime = SDL_GetTicks() + penTime;         //and timer so
                                                    //that ghost is
                                                    //trapped in pen
                                                    //for a bit
}

/*                    reset movement utilities                    */

void Ghost::resetMovements() {
    for (int MOVEMENT = RIGHT; MOVEMENT < MOVE_NUM; MOVEMENT++)
        movements[MOVEMENT] = .5;
}

/*          move towards target at specified coordinates          */

void Ghost::chase(int squareX, int squareY) {

    int xDirection, yDirection;                     //add weights
    if (squareX - pos.x > 0) xDirection = RIGHT;    //to utilities
    if (squareX - pos.x < 0) xDirection = LEFT;     //based on the
    if (squareX - pos.x == 0)                       //ghost's
        xDirection = std::rand() % 2;               //location
    if (squareY - pos.y > 0) yDirection = DOWN;     //relative to
    if (squareY - pos.y < 0) yDirection = UP;       //Pac Man
    if (squareY - pos.y == 0)                       //
        yDirection = std::rand() % 2 + 2;           //notice the
    movements[xDirection] += .1;                    //stochasticity
    movements[yDirection] += .1;                    //when more than
    std::abs(squareX - pos.x) >                     //one value is
        std::abs(squareY - pos.y) ?                 //preferred
        movements[xDirection] += -.05 +
        (8.0 / distance(pos.x, pos.y, squareX, squareY)) :
        movements[yDirection] += -.05 +
        (8.0 / distance(pos.x, pos.y, squareX, squareY));

    if (pos.y == Pacman::teleporters[0].y) {        //add weights
        if (std::abs(Pacman::teleporters[0].x -     //to utilities
            pos.x) >                                //based on the
            std::abs(Pacman::teleporters[1].x -     //ghost's
            pos.x)) movements[RIGHT] += .01;        //location
        if (std::abs(Pacman::teleporters[0].x -     //relative to
            pos.x) <                                //the
            std::abs(Pacman::teleporters[1].x -     //teleporters
            pos.x)) movements[LEFT] += .01;
    }

    checkMap();                                     //add weights
                                                    //to utilities
                                                    //based on the
                                                    //environment

    chosenMovement = std::distance(movements,       //choose a
        std::max_element(movements,                 //direction to
        movements + MOVE_NUM));                     //travel in

    if (portFlag == 0) {                            //set teleporter
        chosenMovement = RIGHT;                     //cooldown if
        portFlag = -1;                              //ghost travels
    }                                               //through it
    if (portFlag == 1) {
        chosenMovement = LEFT;
        portFlag = -1;
    }

    switch (chosenMovement) {                       //move in the
        case RIGHT:                                 //chosen
            pos.x += TILE_SIZE;                     //direction
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

    for(int i = 0; i < Pacman::numTeleporters;      //teleport
        i++) {                                      //ghost if it
		SDL_Rect temp = Pacman::teleporters[i];     //moved into a
		if(temp.x == pos.x && temp.y == pos.y) {    //teleporter
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

/*         use environment for utility weight adjustments         */

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

/* calculate 2D Euclidian distance between specified coordinates  */

double Ghost::distance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

/*             segment ghost images from loaded image             */

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

/*           send updated ghost sprite to window buffer           */

void Ghost::show(int behavior) {
    SDL_RenderCopy(renderer, tex, &(clips[behavior][order]), &pos);
}

/*             create flashing image effect on ghost              */

void Ghost::toggleBehavior() {
    if (SDL_GetTicks() >= toggleTime) {
        toggle = !toggle;
        toggleTime = SDL_GetTicks() + toggleLength;
    }
    show(toggle);
}



/**=================================================================

                              THE END

=================================================================**/
