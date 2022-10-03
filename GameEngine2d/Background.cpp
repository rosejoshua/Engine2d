#include "Background.hpp"

Background::Background()
{
}

Background::Background(SDL_Renderer*& renderer, std::string imgDir, int indexFromBottomOfScreen, int indexInStackIfLayered, bool repeatsHorizontal, bool repeatsVertical)
{
	SDL_Surface* surface = SDL_LoadBMP(imgDir.c_str());
	if (!surface == NULL)
	{
		this->indexFromBottomOfScreen = indexFromBottomOfScreen;
		this->indexInStackIfLayered = indexInStackIfLayered;
		this->width = surface->w;
		this->height = surface->h;
		this->repeatsHorizontal = repeatsHorizontal;
		this->repeatsVertical = repeatsVertical;

		this->renderer = renderer;

		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
		this->texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		drawRect.w = width;
		drawRect.h = height;
	}
}

Background::~Background()
{
	//destroy texture
	SDL_DestroyTexture(texture);
}

int Background::inViewYHeight(int cameraY, int viewHeight, int mapHeight, int tileW, int parallaxFactor, int groundOffset)
{
	return
		height - (((mapHeight * tileW - groundOffset * parallaxFactor) - (cameraY + viewHeight)) / parallaxFactor);
}

void Background::draw(int signedXPos, int signedYPos)
{
	drawRect.x = signedXPos;
	drawRect.y = signedYPos;
	SDL_RenderCopy(renderer, texture, NULL, &drawRect);
	
	//std::cout << "params(cameraX, xWidthToFill, distanceFromGroundOfBottom, viewHeight): " << cameraX << ", " << xWidthToFill << ", " << distanceFromGroundOfViewBottom << ", " << viewHeight << std::endl;

	//std::cout << "srcRect x: " << sourceRect.x << std::endl;
	//std::cout << "srcRect y: " << sourceRect.y << std::endl;
	//std::cout << "srcRect w: " << sourceRect.w << std::endl;
	//std::cout << "srcRect h: " << sourceRect.h << std::endl;
	//std::cout << "drawRect x: " << drawRect.x << std::endl;
	//std::cout << "drawRect y: " << drawRect.y << std::endl;
	//std::cout << "drawRect w: " << drawRect.w << std::endl;
	//std::cout << "drawRect h: " << drawRect.h << std::endl;	
}