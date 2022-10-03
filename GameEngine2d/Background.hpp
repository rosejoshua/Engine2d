#pragma once

// C++ Standard Libraries
#include <string>
#include <vector>
#include <iostream>

// Third-party library
#include "SDL.h"

class Background
{
public:
	Background();
	Background(SDL_Renderer*& renderer, std::string imgDir, int indexFromBottomOfScreen, int indexInStackIfLayered, bool repeatsHorizontal, bool repeatsVertical);
	~Background();

	int indexFromBottomOfScreen;
	int indexInStackIfLayered;
	int width;
	int height;
	bool repeatsHorizontal;
	bool repeatsVertical;

	SDL_Renderer* renderer;
	SDL_Texture* texture;
	//SDL_Rect sourceRect;
	SDL_Rect drawRect;

	int inViewYHeight(int cameraY, int viewHeight, int mapHeight, int tileW, int parallaxFactor, int groundOffset);
	void draw(int signedXPos, int signedYPos);

};

