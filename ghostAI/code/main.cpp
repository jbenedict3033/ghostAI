/**=================================================================

                         ghostAI - main.cpp
                           JOEY BENEDICT
                      ARTIFICIAL INTELLIGENCE

This file is the main driver for the Pac Man game.

=================================================================**/



/**________________   pre-compiler directives   _________________**/

#include <windows.h>
#include <iostream>
#include <sstream>

#include "levelReader.hpp"
#include "sdl.hpp"



/**______________________   declarations   ______________________**/

/*                      no global variables                       */



/**______________________   main program   ______________________**/

int main(int argc, char* argv[]) {

/*                    define objects/variables                    */

    SDL sdl;
	SDL_Event e;
	bool run = true;
	int start_time = SDL_GetTicks();

/*                  create level and vision gui                   */

	LevelReader lr("levels\\default_level.txt");
	sdl.initializeSDL(463, 150);
	Level l = lr.createLevel(sdl);
	l.drawLevel();
	l.createVision(0, 150);



/**____________________   main window loop   ____________________**/

	while (run) {

/*                    check for win condition                     */

        if (l.winningScore()) {
			MessageBox(NULL, "You win!",
                "Congratulations!", MB_OK);
			run = false; break;
		}

/*                    check for loss condition                    */

		else if (l.losingScore()) {
            int gameTime = SDL_GetTicks() - start_time;
            std::stringstream gameLost;
            gameLost << "You lose!\n\nYou lasted for " <<
                gameTime << " ticks\n\n\n";
			MessageBox(NULL, gameLost.str().c_str(),
                "Oh no!", MB_OK);
			run = false; break;
		}

/*                  close windows when prompted                   */

		while (SDL_PollEvent(&e)) {
			l.getPacman()->handleEvents(e);
			if (e.type == SDL_QUIT) {
				run = false; break;
			}
            if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                run = false; break;
            }
		}

/*                   run the actual level loop                    */

		l.levelLoop();
	}
	return 0;
}



/**=================================================================

                              THE END

=================================================================**/
