#pragma once

// C++ Standard Libraries
#include <string>
#include <vector>
#include <iostream>

// Third-party library
#include "SDL.h"

// local library
#include "SpriteManager.hpp"

class PlayerSpriteManager
{
public:

	PlayerSpriteManager(SDL_Renderer* renderer);
	~PlayerSpriteManager();

	const std::string spriteDir = "./sprites/";
	SpriteManager* front;
	SpriteManager* runningLeft;
	SpriteManager* runningRight;
	SpriteManager* runningLeftFromRight;
	SpriteManager* runningRightFromLeft;
	SpriteManager* jumpingLeft;
	SpriteManager* jumpingRight;
	SpriteManager* jumpingFront;
	SpriteManager* landingLeft;
	SpriteManager* landingRight;
	SpriteManager* landingFront;
	SpriteManager* turningLeftFromCenter;
	SpriteManager* turningRightFromCenter;
	SpriteManager* turningFrontFromLeft;
	SpriteManager* turningFrontFromRight;

	void drawSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect, bool airborne, int normalizedDirection);


	//void drawSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect, int xPos, int yPos, Uint64 currentTimeInMillis);
};

