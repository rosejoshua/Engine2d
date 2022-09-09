#pragma once
#include <vector>

#include "MyRGB.hpp"
#include "Bitmap.hpp"
#include "LevelMap.hpp"

class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	const MyRGB black = MyRGB(0, 0, 0);
	const MyRGB white = MyRGB(255, 255, 255);
	const MyRGB middleGray = MyRGB(128, 128, 128);
	const std::string levelDir = "levels/level";
	const std::string levelDirEnd = ".bmp";

	int currentLevel;
	LevelMap* currentLevelMap;

	MyRGB backgroundColor;
	std::vector <MyRGB> tileStainPallete;

	bool loadLevel(int levelNum);
	void restartCurrentLevel();
	void unloadLevel();

};

