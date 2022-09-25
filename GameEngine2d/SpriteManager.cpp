#include "SpriteManager.hpp"

SpriteManager::SpriteManager(std::string spriteDir, std::string seriesDir, SDL_Renderer* renderer)
{
	millisBetweenFrames = 180;
	normalStartIndex = 0;
	specialStartIndex = 1;
	specialStartIndex2 = 0;
	manualSpriteIndex = 0;
	manualSpriteIndex2 = 1;

	int animationIndex = 0;
	SDL_Surface* tempSurfacePointer = SDL_LoadBMP((spriteDir + "/" +
		seriesDir + "/" + seriesDir + "-" + std::to_string(animationIndex) + ".bmp").c_str());

	while (true)
	{
		if (tempSurfacePointer == NULL)
		{
			break;
		}
		else
		{
			while (tempSurfacePointer != NULL)
			{
				SDL_SetColorKey(tempSurfacePointer, SDL_TRUE, SDL_MapRGB(tempSurfacePointer->format, 255, 255, 255));
				textureVector.push_back(SDL_CreateTextureFromSurface(renderer, tempSurfacePointer));
				SDL_FreeSurface(tempSurfacePointer);
				animationIndex++;
				tempSurfacePointer = SDL_LoadBMP((spriteDir + "/" +
					seriesDir + "/" + seriesDir + "-" + std::to_string(animationIndex) + ".bmp").c_str());
			}
		}

	}
	std::cout << "success grabbing sprite animation, size: " << textureVector.size() << std::endl;
}

SpriteManager::SpriteManager(std::string spriteDir, std::string seriesDir, SDL_Renderer* renderer, int millisBetweenFrames)
{
	this->millisBetweenFrames = millisBetweenFrames;
	normalStartIndex = 0;
	specialStartIndex = 1;
	specialStartIndex2 = 0;
	manualSpriteIndex = 0;
	manualSpriteIndex2 = 1;

	int animationIndex = 0;
	SDL_Surface* tempSurfacePointer = SDL_LoadBMP((spriteDir + "/" +
		seriesDir + "/" + seriesDir + "-" + std::to_string(animationIndex) + ".bmp").c_str());

	while (true)
	{
		if (tempSurfacePointer == NULL)
		{
			break;
		}
		else
		{
			while (tempSurfacePointer != NULL)
			{
				SDL_SetColorKey(tempSurfacePointer, SDL_TRUE, SDL_MapRGB(tempSurfacePointer->format, 255, 255, 255));
				textureVector.push_back(SDL_CreateTextureFromSurface(renderer, tempSurfacePointer));
				SDL_FreeSurface(tempSurfacePointer);
				animationIndex++;
				tempSurfacePointer = SDL_LoadBMP((spriteDir + "/" +
					seriesDir + "/" + seriesDir + "-" + std::to_string(animationIndex) + ".bmp").c_str());
			}
		}

	}
	std::cout << "success grabbing sprite animation, size: " << textureVector.size() << std::endl;
}

SpriteManager::~SpriteManager()
{
	for (auto i : textureVector)
	{
		SDL_DestroyTexture(i);;
	}
}

void SpriteManager::drawSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect)
{
	SDL_RenderCopy(renderer, textureVector[
		(SDL_GetTicks64() % (textureVector.size() * millisBetweenFrames))
			/
		(millisBetweenFrames)
	], NULL, p_playerDrawingRect);
}

void SpriteManager::drawSpriteSpecial(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect)
{
	SDL_RenderCopy(renderer, textureVector[
		specialStartIndex +
		(SDL_GetTicks64() % ((textureVector.size() - specialStartIndex) * millisBetweenFrames))
			/
			(millisBetweenFrames)
	], NULL, p_playerDrawingRect);
}

void SpriteManager::drawSingleSprite(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect)
{
	SDL_RenderCopy(renderer, textureVector[
		manualSpriteIndex
	], NULL, p_playerDrawingRect);
}

void SpriteManager::drawSingleSprite2(SDL_Renderer* renderer, SDL_Rect* p_playerDrawingRect)
{
	SDL_RenderCopy(renderer, textureVector[
		manualSpriteIndex2
	], NULL, p_playerDrawingRect);
}