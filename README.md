# <p align="center" markdown="1">ghostAI</p>

### Summary
ghostAI is an artificial intelligence project. The goal is to design an AI for the ghosts in Pac Man that is superior to the original _Midway_ game. In addition, the ideal AI is both efficient and scalable.

Right now, a working game has been implemented with an AI that is legitimately effective at chasing and trapping a user-controlled Pac Man. However, the game's controls and environment are too different from the original to make a direct comparison. In addition, there are a couple bugs that can unexectedly change how the player moves. There are also possible optimizations in the code. Finally, the code itself could be organized and commented better.

That said, this project is still underway and will continue to get better over time.

### Deliverables
The ghostAI folder contains four folders and three files.

###### Folders
- _code_ - contains all the .cpp and .hpp files written specifically for the project
- _graphics_ - contains .bmp files that the project uses as textures for the background and sprites
- _levels_ - contains .txt files that are translated into level layouts
- _objects_ - holds object files for the associated makefile

###### Files
- _ghostAI.exe_ - executable that runs the game as-is (no special features)
- _makefile_ - builds and/or cleans the project
- _SDL2.dll_ - contains the SDL2 library required for the project to run

### Requirements
The ghostAI project builds and runs on Windows operating systems with the following installed:
- mingw 32-bit (SJLJ)
- SDL 2.0.9 (i686 w64 mingw32)

The mingw installer can be found [here](https://osdn.net/projects/mingw/releases/ "mingw Download"). Download `mingw-get-setup.exe` and then follow the guide [here](http://www.mingw.org/wiki/Getting_Started "Getting Started with mingw").
The SDL download can be found [here](http://libsdl.org/download-2.0.php "SDL 2.0.9").

### Setup
ghostAI comes with an executable ready to go. However, to rebuild the ghostAI executable, the existing makefile must be altered:
```
CPATH1 = C:\Users\JBenedict\Documents\6_Essential\Files_Installers
CPATH2 = \code_blocks\SDL_mingw\SDL2-2.0.9\i686-w64-mingw32\include
CPATH3 = \SDL2
CPATH = $(CPATH1)$(CPATH2)$(CPATH3)

LPATH1 = C:\Users\JBenedict\Documents\6_Essential\Files_Installers
LPATH2 = \code_blocks\SDL_mingw\SDL2-2.0.9\i686-w64-mingw32\lib
LPATH = $(LPATH1)$(LPATH2)
```
In the section above, the variables `CPATH1`, `CPATH2`, and `CPATH3` must be changed to the directory of the SDL2 header files, which should look like `i686-w64-mingw32\include\SDL2` at the end. In addition, the variables `LPATH1` and `LPATH2` must be changed to the directory of the SDL2 library files, which should look like `i686-w64-mingw32\lib` at the end.

Once the makefile is altered in this way, navigating to the ghostAI folder in the command prompt and typing `make all` will generate a working executable. Alternatively, typing `make clean` will remove the executable along with all of the object files.

### Execution
The executable can be run either by typing `ghostAI.exe` from the command prompt, or by running the file from Windows Explorer. Once the file starts running, a game of Pac Man immediately begins. The game continues running until one of two conditions are met:
- The game window closes, which causes the console to close as well.
- A win/loss condition is met in game, in which case a message box appears. Selecting `OK` or closing the message box then closes all windows and the console.

### Controls
In the Pac Man game, the user controls the Pac Man character with the arrow keys. `Up` moves Pac Man up, `Down` moves him down, `Right` moves him right, and `Left` moves him left.

### Gameplay
To win the Pac Man game, the Pac Man character must consume all of the round spheres on the map, which are called _pellets_. While doing so, Pac Man is chased by 4 NPCs called _ghosts_. If a ghost catches Pac Man, the user loses the game. The exception to this rule are the large pellets, called _power pellets_. When Pac Man eats a power pellet, he is invulnerable for a brief period of time and any ghosts he touches are consumed and must regenerate before chasing him again.

Some important details to remember:
- The characters' movements are restricted to open corridors within the maze. Neither Pac Man nor the ghosts can move through walls. **However**, Pac Man and the ghosts **can** use a two portal system in the middle of the maze to travel from the left edge of the map to the right and vice-versa.
- When under the effects of a power pellet, Pac Man is **still** vulnerable to ghosts who generate or regenerate (a.k.a. leave the center) **after** the power pellet is consumed.
- Unlike the original game, Pac Man does **not** get a speed boost for preemptively turning around corners.

### Features

###### Environment Toggle
ghostAI features an AI for the ghosts that is based off a combination of Markov Decision Process theory and multi-agent search theory. However, the approach is unique in that the ghosts do not transfer information directly, but use an intermediate, an _environment_ per say. Additionally, the concepts of reward and maximum utility are realized here. Each ghost investigates the environment tiles adjacent to them and heads in the direction with the highest environmental utility (after adjusting for some factors). As a result, the space complexity is reduced because the ghosts do not need to hold information about one another.

To see this environment in action, an optional edit can be made to the file _vision.cpp_ in the _code_ folder. This file contains a constructor for the _vision_ class as follows:
```
Vision::Vision(int numLines, int x, int y) {
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)) {
        std::cout << "sdl not created\n"; SDL_Quit();
    }

	screen = SDL_CreateWindow("Ghost Vision", x, y,
        SDL::SCREEN_WIDTH, SDL::SCREEN_HEIGHT, SDL_WINDOW_HIDDEN);
    if(!screen) {
	    std::cout << "failed to create screen\n"; SDL_Quit();
    }
    renderer = SDL_CreateRenderer(screen, -1, 0);
    if(!renderer) {
        std::cout << "failed to create renderer\n"; SDL_Quit();
    }
    SDL_SetRenderDrawColor(renderer, 37, 160, 30, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    map_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET, SDL::SCREEN_WIDTH,
        SDL::SCREEN_HEIGHT);
	lineCount = numLines;
	lineLength = LevelReader::lineLength;
	tile.w = tile.h = TILE_SIZE;
}
```
If the argument `SDL_WINDOW_HIDDEN` in the function `SDL_CreateWindow` is changed to `0`, a second window will appear after running the executable (after rebuilding the project). This window serves as a GUI by showing how the ghosts' movement is captured by the environment. Tiles that are more red in color have a lower utility than tiles that are less red. Black tiles represent prohibited areas in the map.

###### Level Selection
The project comes with a small number of levels to play. To switch the level, open _main.cpp_ in the _code_ folder and navigate to the following selection:
```
/*                  create level and vision gui                   */

	LevelReader lr("levels\\default_level.txt");
	sdl.initializeSDL(463, 150);
	Level l = lr.createLevel(sdl);
	l.drawLevel();
	l.createVision(0, 150);
```
In the constructor for `lr`, replace `default_level.txt` with the name of any other filename in the _levels_ folder. Beware, some levels are harder than others!

###### Time of Survival
This feature is passive and does not currently turn off. The game tracks how long each program execution lasts. If the user loses the game, his or her total time is displayed in the resulting message box. The time is measured in _ticks_, which vary in length from system to system, but within a certain system, they are a reliable way of measuring progress towards completing a level.

### Limitations
There are several limitations with this project:
- The ghosts' movements can be easier to predict because each ghost is coded identically (as opposed to the original game, which gave each ghost unique "personality traits").
- The characters all move by frame (rather than by pixel in the original game).
- Pac Man can occasionally stop moving instead of turning, due to both programming bugs and possibly hardware limitations (some keyboards will not register more than 2 pressed down arrow keys at once).
- There is no level progression system and only one "life" for Pac Man. Once the user beats or loses the level, the game ends.
- The speed at which the game plays is generally faster than the original game, but can very depending on the system.
- There is no start menu, so the game starts so suddenly that it catches some users by surprise.
- In very rare cases, either _Blinky_ (the red ghost) or _Clyde_ (the orange ghost) can remain invulnerable even after Pac Man eats a power pellet. This does not include cases where the ghosts leave the center after Pac Man eats the power pellet.
- There are no _fruits_, which were rewards in the original game.
- There is no scoring system outside of seeing how long a player can last, or whether they win or not.
- The console can be seen displaying miscellaneous information in the background during gameplay.

This may not be an exhaustive list, and will be updated as more limitations become known.

### Credits
This project was undertaken individually. However, the code files are inspired by an open source Pac Man project by [Braden Watling](https://github.com/bradenwatling/Pacman "Pac Man by Braden Watling"). Please note, however, that this other project was written using the SDL1 library and did not contain any ghosts whatsoever. Therefore, it made sense for ghostAI to be a separate project instead of a fork.

During the formation of this project, Dr. Jonathan Mwaura from the University of Massachusetts Lowell provided insightful feedback and recommendations that steered both the concepts and code in the right direction.

### Copyright
Copyright Joey Benedict - All Rights Reserved

Unauthorized copying of this file, via any medium is strictly prohibited.

Written by Joey Benedict, 2019
