#include <iostream>
#include <iomanip>

#include "level.hpp"

enum {
	doubleTopLeft = '/',        doubleTopRight = '0',
	doubleBottomLeft = '1',     doubleBottomRight = '2',
	singleTopLeft = '3',        singleTopRight = '4',
	singleBottomLeft = '5',     singleBottomRight = '6',
	squareTopLeft = '7',        squareTopRight = '8',
	squareBottomLeft = '9',     squareBottomRight = ':',
	doubleHorizontal = ';',     singleHorizontal = '<',
	doubleVertical = '=',       singleVertical = '>',
	dot = '.',                  bigDot = 'o',
	empty = ' ',                unknown = '?',
	caseGhost = 'g',            casePacman = 'P',
	innerDoor = '@',            outerDoor = 'd',
	teleporter = 't'
};

unsigned int Level::ghostTime = 0;
int Level::ghostOrder = 0;

Level::Level(int numLines, std::map<int,std::string> fileText,
    SDL Sdl) {
	text = fileText;
	sdl = Sdl;
	lineCount = numLines;
	lineLength = LevelReader::lineLength;
	dotNum = bigDotNum = teleporterNum = innerDoorNum =
        outerDoorNum = gameLost = 0;
	ghostSpeedHandicap = 120;
	ghostTime = SDL_GetTicks() + ghostSpeedHandicap;
	Ghost::wallArray = std::vector<std::vector<float> >(lineCount + 1);
	for (int height = 0; height <= lineCount; height++) {
        Ghost::wallArray[height] = std::vector<float>(lineLength);
        for (int width = 0; width < lineLength; width++) {
            Ghost::wallArray[height][width] = .5;
        }
    }
    setWallClips();
}

void Level::addToWalls(int x, int y) {
    Ghost::wallArray[y / TILE_SIZE][x / TILE_SIZE] = -1;
}

void Level::addToOuterDoors(int x, int y) {
    Ghost::outsideDoor[outerDoorNum].x = x;
    Ghost::outsideDoor[outerDoorNum].y = y;
    outerDoorNum++;
}

void Level::addToInnerDoors(int x, int y) {
    Ghost::insideDoor[innerDoorNum].x = x;
    Ghost::insideDoor[innerDoorNum].y = y;
    innerDoorNum++;
}

void Level::addToDots(int x, int y) {
	if(dotNum < (int)sizeof(dots)) {
		dots[dotNum].x = x;
		dots[dotNum].y = y;
		dots[dotNum].w = dots[dotNum].h = TILE_SIZE;
		dotNum++;
	}
}

void Level::addToBigDots(int x, int y) {
	if(bigDotNum < (int)sizeof(bigDots)) {
		bigDots[bigDotNum].x = x;
		bigDots[bigDotNum].y = y;
		bigDots[bigDotNum].w = TILE_SIZE;
		bigDots[bigDotNum].h = TILE_SIZE;
		bigDotNum++;
	}
}

void Level::addToTeleporters(int x, int y) {
	if(teleporterNum < (int)sizeof(teleporters)) {
		teleporters[teleporterNum].x = x;
		teleporters[teleporterNum].y = y;
		teleporters[teleporterNum].w = TILE_SIZE;
		teleporters[teleporterNum].h = TILE_SIZE;
		teleporterNum++;
	}
}

int Level::getDotNum() { return dotNum; }

int Level::getBigDotNum() { return bigDotNum; }

int Level::getTeleporterNum() { return bigDotNum; }

Pacman* Level::getPacman() { return pacman; }

Ghost** Level::getGhostGang() { return ghostGang; }

void Level::createVision(int x, int y) {
    vision = new Vision(lineCount, x, y);
    vision->drawVision();
}

void Level::updateMap(Ghost* ghost) {
    if (Ghost::wallArray[ghost->getPos().y / TILE_SIZE]
        [ghost->getPos().x / TILE_SIZE] > .1) {
        Ghost::wallArray[ghost->getPos().y / TILE_SIZE]
        [ghost->getPos().x / TILE_SIZE] -= .1;
    }
}

void Level::setWallClips() {
    for (int i = 0; i < 18; i++) {
        wallClips[i].x = TILE_SIZE * i;
        wallClips[i].y = 0;
        wallClips[i].w = wallClips[i].h = TILE_SIZE;
    }
}

void Level::drawLevel() {
	std::string line;
	std::map<int, std::string>::iterator iter;
	for(int y = 0; y <= lineCount; ++y) {
		iter = text.find(y);
		line = iter->second;
		handleLine(line, y);
	}
}

void Level::handleLine(std::string line, int y) {
	int X = 0;
	int Y = y * TILE_SIZE;
	for(unsigned int x = 0; x < line.length(); ++x) {
		X = x * TILE_SIZE;
		switch(line[x]) {
			case dot:
				sdl.loadBMP("graphics\\dot.bmp",
                X, Y, sdl.getLevelTexture(), wallClips[0]);
				addToDots(X, Y);
				break;
			case bigDot:
				sdl.loadBMP("graphics\\big_dot.bmp",
                X, Y, sdl.getLevelTexture(), wallClips[0]);
				addToBigDots(X, Y);
				break;
            case outerDoor:
                addToOuterDoors(X, Y);
            case empty:
				sdl.loadBMP("graphics\\empty.bmp",
                X, Y, sdl.getLevelTexture(), wallClips[0]);
				break;
			case teleporter:
				addToTeleporters(X, Y);
				break;
            case casePacman:
                pacman = new Pacman(X, Y,
                    "graphics\\pacman.bmp",
                    sdl.getLevelTexture(), sdl.getEmptyTexture(),
                    sdl.getRenderer());
                break;
            case caseGhost:
                ghostGang[ghostOrder] = new Ghost(X, Y,
                    "graphics\\ghosts.bmp", sdl.getRenderer(),
                    ghostOrder);
                    ghostOrder++;
                break;
            case innerDoor:
                addToInnerDoors(X, Y);
            default:
                sdl.loadBMP("graphics\\walls.bmp", X, Y,
                sdl.getLevelTexture(), wallClips[line[x] - 47]);
				addToWalls(X, Y);
				break;
		}
	}
	Pacman::updateDots(dots, dotNum);
	Pacman::updateBigDots(bigDots, bigDotNum);
	Pacman::updateTeleporters(teleporters, teleporterNum);
}

bool Level::winningScore() {
    return getPacman()->getPoints() == Pacman::numDots +
            Pacman::numBigDots * 5;
}

bool Level::losingScore() {
    return gameLost;
}

void Level::levelLoop() {
    getPacman()->move();
    getPacman()->show();
    ghostGangLoop();
    SDL_RenderPresent(sdl.getRenderer());
}

void Level::ghostGangLoop() {
    if (pacman->pacIsPowerful()) ghostSpeedHandicap = 240;
    else ghostSpeedHandicap = 120;
    bool updated, hitPacman;
    for (int GHOST = BLINKY; GHOST <=CLYDE; GHOST++) {
        hitPacman = ghostGang[GHOST]-> ghostLoop(pacman->getPosX(),
            pacman->getPosY(), ghostTime, updated,
            pacman->pacIsPowerful(), pacman->pacAtePower());
        if (hitPacman) gameLost = 1;
        if (updated) updateMap(ghostGang[GHOST]);
    }
    if (gameLost) return;
    if (updated) {
        ghostTime = SDL_GetTicks() + ghostSpeedHandicap;
        vision->updateVision();
        pacman->atePowerDone();
    }
}
