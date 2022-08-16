#include "TextureTile.hpp"

TextureTile::TextureTile(int tileW)
{
	isAnimated = false;
	hasAltTransparency = false;
	collisionAnimation = false;
	lastDrawTime = SDL_GetTicks64();
	desiredMillisBetweenAnimationFrames = 100;
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
