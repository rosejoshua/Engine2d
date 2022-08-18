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

	bool isCollision;
	bool drawsInFront;
	bool isLethal;
	bool hasSpecialCharacteristics;
	bool isAnimated;
	bool isConsumable;
	bool isLiquid;
	bool hasCollisionAnimation;
	bool hasTransformation;
	bool hasMinimumCollisionRatio;
	int transformationTileIndex;
	int transparencyAlphaValue;
	int desiredMillisBetweenAnimationFrames;
	Uint64 lastDrawTime;
	SDL_Rect rectangle;
	std::vector<SDL_Texture*> textureVector;
	void setRectPos(int xPos, int yPos);
	void render(SDL_Renderer*& renderer);
};

