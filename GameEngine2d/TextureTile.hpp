#pragma once

// C++ Standard Libraries
#include <string>
#include <vector>
#include <iostream>

// Third-party library
#include "SDL.h"


class TextureTile
{
public:
	TextureTile(int tileW);
	~TextureTile();

	bool isAnimated;
	bool hasAltTransparency;
	bool collisionAnimation;
	Uint64 lastDrawTime;
	int desiredMillisBetweenAnimationFrames;
	SDL_Rect rectangle;
	std::vector<SDL_Texture*> textureVector;
	void setRectPos(int xPos, int yPos);
	void render(SDL_Renderer*& renderer);
};

