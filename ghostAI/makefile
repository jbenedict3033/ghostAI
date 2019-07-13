#===================================================================
#
#						 ghostAI - makefile
#						   JOEY BENEDICT
#					  ARTIFICIAL INTELLIGENCE
#
#===================================================================



#_________________________   variables   ___________________________

CC = g++

FLAGS = -Wall -Werror -pedantic -ansi -std=c++11

CODEDIR = code

OBJDIR = objects

CODE_FILES = $(wildcard $(CODEDIR)/*.cpp)

OBJECTS = $(patsubst $(CODEDIR)/%.cpp, $(OBJDIR)/%.o, $(CODE_FILES))

BUILT_OBJECTS = $(OBJDIR)\*.o

HEADERS = $(wildcard $(CODEDIR)/*.hpp)

LIBS = -lmingw32 -lSDL2main -lSDL2

OUTPUT = ghostAI.exe

CPATH1 = C:\Users\JBenedict\Documents\6_Essential\Files_Installers
CPATH2 = \code_blocks\SDL_mingw\SDL2-2.0.9\i686-w64-mingw32\include
CPATH3 = \SDL2
CPATH = $(CPATH1)$(CPATH2)$(CPATH3)

LPATH1 = C:\Users\JBenedict\Documents\6_Essential\Files_Installers
LPATH2 = \code_blocks\SDL_mingw\SDL2-2.0.9\i686-w64-mingw32\lib
LPATH = $(LPATH1)$(LPATH2)



#__________________________   targets   ____________________________

all: ghostAI

ghostAI: $(OBJECTS)
	$(CC) $(FLAGS) -o $(OUTPUT) $(OBJECTS) -L$(LPATH) $(LIBS)

$(OBJDIR)/%.o: $(CODEDIR)/%.cpp $(HEADERS)
	$(CC) $(FLAGS) -o $@ -c $< -I $(CPATH)

clean:
	del $(OUTPUT) $(BUILT_OBJECTS)



#________________________   other stuff   __________________________

.PHONY: all clean


#===================================================================
#
#							  THE END
#
#===================================================================
