#ifndef VISION_HPP
#define VISION_HPP

#include <string>
#include <map>
#include <vector>

#include "sdl.hpp"
#include "pacman.hpp"
#include "levelReader.hpp"
#include "ghost.hpp"

class Vision {
public:
    Vision(int numLines, int x, int y);
    void drawVision();
    void updateVision();
    void showVision();
    void renderPresent();
    void visionLoop();
    SDL_Renderer* getRenderer();
private:
    SDL_Window* screen;
    SDL_Renderer* renderer;
    SDL_Texture* map_tex;
    SDL_Rect tile;
    int lineCount;
    int lineLength;
};

#endif                                              //VISION_HPP
