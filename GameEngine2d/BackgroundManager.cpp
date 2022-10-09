#include "BackgroundManager.hpp"

BackgroundManager::BackgroundManager()
{
}

BackgroundManager::BackgroundManager(SDL_Renderer*& renderer, std::string imgDir, int parallaxXRatio, int parallaxYRatio, int mapHeight, int tileW)
{
	this->parallaxXRatio = parallaxXRatio;
	this->parallaxYRatio = parallaxYRatio;
	this->mapHeight = mapHeight;
	this->tileW = tileW;
	background0 = new Background(renderer, imgDir + "mountain_background.bmp", 0, 0, true, false);
	background1 = new Background(renderer, imgDir + "sky_background.bmp", 1, 1, true, true);
}

BackgroundManager::~BackgroundManager()
{
	delete background0;
	delete background1;
}

void BackgroundManager::drawBackground(int cameraX, int cameraY, int resW, int resH)
{
	int cloudParallaxModifier = 4;
	int currentYCheck0 = background0->inViewYHeight(cameraY, resH, mapHeight, tileW, parallaxYRatio, 0);
	if (currentYCheck0 > 0)
	{
		background0->draw(-((cameraX / (parallaxXRatio - cloudParallaxModifier)) % background0->width), resH - currentYCheck0);
		if (-((cameraX / (parallaxXRatio - cloudParallaxModifier)) % background0->width) + background0->width < resW)
			background0->draw(-((cameraX / (parallaxXRatio - cloudParallaxModifier)) % background0->width) + background0->width, resH - currentYCheck0);
	}
	int currentYCheck1 = background1->inViewYHeight(cameraY, resH, mapHeight, tileW, parallaxYRatio + cloudParallaxModifier, background0->height);
	if (currentYCheck1 > 0)
	{
		background1->draw(-((cameraX / parallaxXRatio + cloudParallaxModifier) % background1->width), resH - currentYCheck1);
		if (-((cameraX / parallaxXRatio + cloudParallaxModifier) % background1->width) + background1->width < resW)
			background1->draw(-((cameraX / parallaxXRatio + cloudParallaxModifier) % background1->width) + background1->width, resH - currentYCheck1);
	}

	for (size_t i = 0; i < 2; i++)
	{
		int currentYCheck2 = background1->inViewYHeight(cameraY, resH, mapHeight, tileW, parallaxYRatio + cloudParallaxModifier, background0->height + i*background1->height);
		if (currentYCheck2 > 0)
		{
			background1->draw(-((cameraX / parallaxXRatio + cloudParallaxModifier) % background1->width), resH - currentYCheck2);
			if (-((cameraX / parallaxXRatio + cloudParallaxModifier) % background1->width) + background1->width < resW)
				background1->draw(-((cameraX / parallaxXRatio + cloudParallaxModifier) % background1->width) + background1->width, resH - currentYCheck2);
		}
	}

	
}
