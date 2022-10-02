#pragma once

// C++ Standard Libraries
#include <string>
#include <vector>
#include <iostream>

// Third-party library
#include "SDL.h"
#include "Background.hpp"

class BackgroundManager
{
public:
	BackgroundManager();
	BackgroundManager(SDL_Renderer*& renderer, std::string imgDir, int parallaxXRatio, int parallaxYRatio, int mapHeight, int tileW);
	~BackgroundManager();

	int parallaxXRatio;
	int parallaxYRatio;
	int mapHeight;
	int tileW;

	Background* background0;
	Background* background1;

	void drawBackground(int cameraX, int cameraY, int resW, int resH);
};


