#pragma once

// C++ Standard Libraries
#include <string>
#include <vector>
#include <iostream>

// Third-party library
#include "SDL.h"

class SpriteManager
{
public:

	SpriteManager(std::string spriteDir, std::string seriesDir, SDL_Renderer* renderer);
	SpriteManager(std::string spriteDir, std::string seriesDir, SDL_Renderer* renderer, int millisBetweenFrames);
	~SpriteManager();

	int millisBetweenFrames;
	int normalStartIndex;
	int specialStartIndex;
	int specialStartIndex2;
	int manualSpriteIndex;
	int manualSpriteIndex2;
	std::vector<SDL_Texture*> textureVector;

	void drawSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect);
	void drawSpriteSpecial(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect);
	void drawSingleSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect);
	void drawSingleSprite2(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect);
};

