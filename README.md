# <p align="center" markdown="1">ghostAI</p>

### Summary
ghostAI is an artificial intelligence project. The goal is to design an AI for the ghosts in Pac Man that is superior to the original _Midway_ game. In addition, the ideal AI is both efficient and scalable.

Right now, a working game has been implemented with an AI that is legitimately effective at chasing and trapping a user-controlled Pac Man. However, the game's controls and environment are too different from the original to make a direct comparison. In addition, there are a couple bugs that can unexectedly change how the player moves. There are also possible optimizations in the code. Finally, the code itself could be organized and commented better.

That said, this project is still underway and will continue to get better over time.

### Deliverables
The ghostAI folder contains four folders and two files.

###### Folders
- _code_ - contains all the .cpp and .hpp files written specifically for the project
- _graphics_ - contains .bmp files that the project uses as textures for the background and sprites
- _levels_ - contains .txt files that are translated into level layouts
- _objects_ - empty when downloaded, but holds object files when the associated makefile is used

###### Files
- _makefile_ - builds the project and drops the executable into the ghostAI folder
- _SDL2.dll_ - contains the SDL2 library required for the project to run

### Requirements
The ghostAI project runs on Windows operating systems with the following installed:
- mingw 32-bit (SJLJ)
- SDL 2.0.9 (i686 w64 mingw32)

The mingw installer can be found [here](https://osdn.net/projects/mingw/releases/ "mingw Download"). Download `mingw-get-setup.exe` and then follow the guide [here](http://www.mingw.org/wiki/Getting_Started "Getting Started with mingw").
The SDL download can be found [here](http://libsdl.org/download-2.0.php "SDL 2.0.9").

### Setup
To build the ghostAI executable, the existing makefile must be altered:
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

Once the makefile is altered in this way, navigating to the ghostAI folder in the command prompt and typing `make all` will generate a working executable.

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
