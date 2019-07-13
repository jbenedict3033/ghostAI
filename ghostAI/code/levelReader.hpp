#ifndef LEVELREADER_HPP
#define LEVELREADER_HPP

#include <string>
#include <fstream>
#include <map>

#include "level.hpp"
#include "vision.hpp"

class Vision;

class LevelReader
{
public:
	LevelReader(std::string path);
	Level createLevel(SDL sdl);

	static int lineCount;
	static int lineLength;

private:
	std::ifstream stream;
	std::map<int, std::string> lines;
};

#endif                                              //LEVELREADER_HPP
