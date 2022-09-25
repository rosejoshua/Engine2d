#include "PlayerSpriteManager.hpp"

PlayerSpriteManager::PlayerSpriteManager(SDL_Renderer* renderer)
{
	front = new SpriteManager(spriteDir, "front", renderer);
	jumpingFront = new SpriteManager(spriteDir, "jump_front", renderer);
	runningRight = new SpriteManager(spriteDir, "right", renderer, 50);
	runningLeft = new SpriteManager(spriteDir, "left", renderer, 50);
	jumpingRight = new SpriteManager(spriteDir, "jump_right", renderer, 50);
	jumpingLeft = new SpriteManager(spriteDir, "jump_left", renderer, 50);
	
}

PlayerSpriteManager::~PlayerSpriteManager()
{
	delete front;
	delete jumpingFront;
}

void PlayerSpriteManager::drawSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect, bool airborne, int normalizedDirection)
{
	if (airborne)
	{
		if (normalizedDirection == 0)
		jumpingFront->drawSingleSprite(renderer, p_playerDrawingRect);
		else
		{
			if (normalizedDirection == 1)
				jumpingRight->drawSingleSprite(renderer, p_playerDrawingRect);
			else
				jumpingLeft->drawSingleSprite(renderer, p_playerDrawingRect);
		}
	}
	else
	{
		if (normalizedDirection == 0)
			front->drawSprite(renderer, p_playerDrawingRect);
		else
		{
			if (normalizedDirection == 1)
				runningRight->drawSprite(renderer, p_playerDrawingRect);
			else
				runningLeft->drawSprite(renderer, p_playerDrawingRect);
		}
	}
}