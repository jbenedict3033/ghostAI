#include <iostream>
#include <iomanip>

#include "level.hpp"
#include "pacman.hpp"

SDL_Rect* Pacman::dots = new SDL_Rect();
SDL_Rect* Pacman::bigDots = new SDL_Rect();
SDL_Rect* Pacman::teleporters = new SDL_Rect();
int Pacman::numDots = 0;
int Pacman::numBigDots = 0;
int Pacman::numTeleporters = 0;
int Pacman::pacTime = 0;

Pacman::Pacman(int X, int Y, std::string path,
    SDL_Texture* ScreenCache, SDL_Texture* EmptyTile,
    SDL_Renderer* Renderer) {
    renderer = Renderer;
    emptyTile = EmptyTile;
    screenCache = ScreenCache;
	setClips();
    pos.x = X;
	pos.y = Y;
	pos.w = pos.h = TILE_SIZE;
    frame = points = pacHasPower = keysCount = status = 0;
	loadImage(path);
	pacTex = SDL_CreateTextureFromSurface(renderer, pacman);
    if (!pacTex) { std::cout << "pacman texture not made\n"; exit(1); }
	pacSpeedHandicap = 100;
	pacTime = SDL_GetTicks() + pacSpeedHandicap;
	powerLength = 5000;
	for (int i = 0; i < MOVE_NUM; i++) keysDown[i] = 0;
}

void Pacman::updateDots(SDL_Rect* Dots, int numdots) {
	dots = Dots;
	numDots = numdots;
}

void Pacman::updateBigDots(SDL_Rect* BigDots, int numbigdots) {
	bigDots = BigDots;
	numBigDots = numbigdots;
}

void Pacman::updateTeleporters(SDL_Rect* Teleporters,
    int numteleporters) {
	teleporters = Teleporters;
	numTeleporters = numteleporters;
}

int Pacman::getPoints() { return points; }

bool Pacman::pacAtePower() { return atePower; }

void Pacman::atePowerDone() { atePower = false; }

void Pacman::loadImage(std::string path) {
	SDL_Surface* loadedImage = NULL;
    loadedImage = SDL_LoadBMP(path.c_str());
    if(!loadedImage) {
        std::cout << "could not load pacman image\n"; exit(1);
    }
    pacman = loadedImage;
}

void Pacman::handleEvents(SDL_Event ev) {
    int keyUpNumber;
	if(ev.type == SDL_KEYDOWN) {
		switch(ev.key.keysym.sym) {
			case SDLK_RIGHT:
				if (keysDown[RIGHT] == 0) {
                    keysCount++;
                    keysDown[RIGHT] = keysCount;
				}
				break;
			case SDLK_LEFT:
				if (keysDown[LEFT] == 0) {
                    keysCount++;
                    keysDown[LEFT] = keysCount;
				}
				break;
			case SDLK_DOWN:
				if (keysDown[DOWN] == 0) {
                    keysCount++;
                    keysDown[DOWN] = keysCount;
				}
				break;
			case SDLK_UP:
				if (keysDown[UP] == 0) {
                    keysCount++;
                    keysDown[UP] = keysCount;
				}
				break;
		}
	}
	if(ev.type == SDL_KEYUP) {
        switch(ev.key.keysym.sym) {
            case SDLK_RIGHT:
                keyUpNumber = keysDown[RIGHT];
                keysDown[RIGHT] = 0;
                break;
            case SDLK_LEFT:
                keyUpNumber = keysDown[LEFT];
                keysDown[LEFT] = 0;
                break;
            case SDLK_DOWN:
                keyUpNumber = keysDown[DOWN];
                keysDown[DOWN] = 0;
                break;
            case SDLK_UP:
                keyUpNumber = keysDown[UP];
                keysDown[UP] = 0;
                break;
        }
        keysCount--;
        for (int i = 0; i < MOVE_NUM; i++) {
            if (keysDown[i] > keyUpNumber) keysDown[i]--;
        }
	}
}

void Pacman::move() {
    if (SDL_GetTicks() < (unsigned)pacTime) return;
    for (int i = 0; i < MOVE_NUM; i++) {
        movements[i] = keysDown[i];
    }
    prevMaxIndex = maxValue = 0;
    for (int i = 0; i < MOVE_NUM; i++) {
        std::cout << movements[i];
        if (movements[i] > maxValue) {
            prevMaxIndex = i;
            maxValue = movements[i];
        }
	}
	std::cout << "\tmaxIndex is " << maxIndex << "\t";
	std::cout << "maxValue is " << maxValue << "\t";//*/
    maxIndex = maxValue = 0;
	collision();
	for (int i = 0; i < MOVE_NUM; i++) {
        std::cout << movements[i];
        if (movements[i] > maxValue) {
            maxIndex = i;
            maxValue = movements[i];
        }
	}
	std::cout << "\tmaxIndex is " << maxIndex << "\t";
	std::cout << "maxValue is " << maxValue << "\t";
	if (keysCount > 0) {
        if (maxValue > 0) status = maxIndex;
        else status = prevMaxIndex;
	}
    if ((keysCount > 0) && (maxValue > 0)) {
        switch (maxIndex) {
            case RIGHT:
                std::cout << "moving right\n\n";
                pos.x += TILE_SIZE;
                break;
            case LEFT:
                std::cout << "moving left\n\n";
                pos.x -= TILE_SIZE;
                break;
            case DOWN:
                std::cout << "moving down\n\n";
                pos.y += TILE_SIZE;
                break;
            case UP:
                std::cout << "moving up\n\n";
                pos.y -= TILE_SIZE;
                break;
        }
    }
    else std::cout << "not moving\n\n";
}

void Pacman::show() {
    if (SDL_GetTicks() >= (unsigned)pacTime) {
        if (keysCount > 0) {
            frame++;
        }
        else {
            frame = 0;
        }

        if(frame >= 3) {
            frame = 0;
        }
        pacTime = SDL_GetTicks() + pacSpeedHandicap;
    }
	drawImage(clips[status][frame]);
}

void Pacman::drawImage(SDL_Rect clip) {
	SDL_RenderCopy(renderer, screenCache, NULL, NULL);
    SDL_RenderCopy(renderer, pacTex, &clip, &pos);
}

void Pacman::collision() {
	SDL_Rect temp;
    if ((Ghost::wallArray[pos.y / TILE_SIZE]
        [(pos.x / TILE_SIZE) + 1] < 0) ||
        (pos.x + TILE_SIZE >= SDL::SCREEN_WIDTH)) {
            movements[RIGHT] = 0;
            if (prevMaxIndex == RIGHT)
                movements[LEFT] = 0;
            std::cout << "R ";
        }
	if ((Ghost::wallArray[pos.y / TILE_SIZE]
        [(pos.x / TILE_SIZE) - 1] < 0) ||
        (pos.x - TILE_SIZE < 0)) {
            movements[LEFT] = 0;
            if (prevMaxIndex == LEFT)
                movements[RIGHT] = 0;
            std::cout << "L ";
        }
    if (Ghost::wallArray[(pos.y / TILE_SIZE) + 1]
        [pos.x / TILE_SIZE] < 0) {
            movements[DOWN] = 0;
            if (prevMaxIndex == DOWN)
                movements[UP] = 0;
            std::cout << "D ";
        }
    if (Ghost::wallArray[(pos.y / TILE_SIZE) - 1]
        [pos.x / TILE_SIZE] < 0) {
            movements[UP] = 0;
            if (prevMaxIndex == UP)
                movements[DOWN] = 0;
            std::cout << "U ";
        }
    std::cout << "\tcollision\t";

	for(int i = 0; i < numDots; ++i) {
		temp = dots[i];
		if(temp.x == pos.x && temp.y == pos.y) {
		    SDL_SetRenderTarget(renderer, screenCache);
            SDL_RenderCopy(renderer, emptyTile, NULL, &temp);
            SDL_SetRenderTarget(renderer, NULL);
			points ++;
			dots[i] = SDL_Rect();
			break;
		}
	}


	for(int i = 0; i < numBigDots; ++i) {
		temp = bigDots[i];
		if(temp.x == pos.x && temp.y == pos.y) {

			SDL_SetRenderTarget(renderer, screenCache);
            SDL_RenderCopy(renderer, emptyTile, NULL, &temp);
            SDL_SetRenderTarget(renderer, NULL);
			points += 5;
			bigDots[i] = SDL_Rect();
			pacHasPower = 2;
			powerTime = SDL_GetTicks() + powerLength;
			atePower = true;
			break;
		}
	}

	for(int i = 0; i < numTeleporters; i++) {
		temp = teleporters[i];
		if((temp.x == pos.x) && (temp.y == pos.y) &&
            ((pos.x - TILE_SIZE < 0 &&
                status == LEFT) ||
            (pos.x + TILE_SIZE >= SDL::SCREEN_WIDTH &&
                status == RIGHT))) {
			if(i == 0) {
				pos.x = teleporters[1].x;
				pos.y = teleporters[1].y;
                return;
			}
			else if (i == 1) {
				pos.x = teleporters[0].x;
				pos.y = teleporters[0].y;
			}
		}
	}
}

void Pacman::setClips() {
	for(int i = 0; i < 4; ++i) {
		for(int x = 0; x < 3; ++x) {
			clips[i][x].x = TILE_SIZE * x;
			clips[i][x].y = TILE_SIZE * i;
			clips[i][x].w = TILE_SIZE;
			clips[i][x].h = TILE_SIZE;
		}
	}
}

int Pacman::pacIsPowerful() {
    if (SDL_GetTicks() >= (unsigned)powerTime)
        pacHasPower = 0;
    else if ((SDL_GetTicks() + 2500) >= (unsigned)powerTime)
        pacHasPower = 1;
    return pacHasPower;
}

int Pacman::getPosX() {
    return pos.x;
}

int Pacman::getPosY() {
    return pos.y;
}
