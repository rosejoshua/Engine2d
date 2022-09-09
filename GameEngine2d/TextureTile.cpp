#include "TextureTile.hpp"

TextureTile::TextureTile(int tileW)
{
	isCollision = false;
	drawsInFront = false;
	isLethal = false;
	hasSpecialCharacteristics = false;
	isAnimated = false;
	isConsumable = false;
	isLiquid = false;
	hasCollisionAnimation = false;
	hasTransformation = false;
	tileTransformationBehaviorIndex = 0;
	minimumCollisionRatio = 0;
	transformationTileIndex = 1;
	transparencyAlphaValue = 255;
	desiredMillisBetweenAnimationFrames = 30;
	lastDrawTime = SDL_GetTicks64();
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = tileW;
	rectangle.h = tileW;
}

TextureTile::~TextureTile()
{
	for (auto i : textureVector)
	{
		SDL_DestroyTexture(i);
	}
}

void TextureTile::setRectPos(int xPos, int yPos)
{
	rectangle.x = xPos;
	rectangle.y = yPos;
}

void TextureTile::render(SDL_Renderer*& renderer) {
	SDL_RenderCopy(renderer, textureVector[0], NULL, &rectangle);
}
