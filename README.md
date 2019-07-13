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
The ghostAI project works on Windows operating systems with the following installed:
- mingw 32-bit (SJLJ)
- SDL 2.0.9 (i686 w64 mingw32)

The mingw installer can be found [here](https://osdn.net/projects/mingw/releases/). Download `mingw-get-setup.exe` and then follow the guide [here](http://www.mingw.org/wiki/Getting_Started).
The SDL download can be found [here](http://libsdl.org/download-2.0.php).
