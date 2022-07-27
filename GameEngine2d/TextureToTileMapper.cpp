#include "TextureToTileMapper.hpp"

TextureToTileMapper::TextureToTileMapper(SDL_Renderer*& renderer, int tileW)
{
	int fileIndex = 0;
	int animationIndex = 0;
	TextureTile* textureTilePtr = new TextureTile(tileW);
	SDL_Surface* tempSurfacePointer = SDL_LoadBMP((imagesDir + std::to_string(fileIndex) + "/" +
				std::to_string(tileW) + "-" + std::to_string(animationIndex) + ".bmp").c_str());

	while (true)
	{
		if (tempSurfacePointer == NULL)
		{
			fileIndex++;
			tempSurfacePointer = SDL_LoadBMP((imagesDir + std::to_string(fileIndex) + "/" +
				std::to_string(tileW) + "-" + std::to_string(animationIndex) + ".bmp").c_str());

			if (tempSurfacePointer == NULL)
			{
				break;
			}
		}
		else
		{
			while (tempSurfacePointer != NULL)
			{
				SDL_SetColorKey(tempSurfacePointer, SDL_FALSE, SDL_MapRGB(tempSurfacePointer->format, 254, 254, 254));
				textureTilePtr->textureVector.push_back(SDL_CreateTextureFromSurface(renderer, tempSurfacePointer));
				SDL_FreeSurface(tempSurfacePointer);
				animationIndex++;
				tempSurfacePointer = SDL_LoadBMP((imagesDir + std::to_string(fileIndex) + "/" +
					std::to_string(tileW) + "-" + std::to_string(animationIndex) + ".bmp").c_str());
			}
			animationIndex = 0;
			intToTextureTileVector.push_back(textureTilePtr);
			textureTilePtr = new TextureTile(tileW);
		}
		
	}
	std::cout << "about to exit TTTM ctor," << std::endl;
	std::cout << "ITTTV size, " << intToTextureTileVector.size() << std::endl;
	std::cout << "size in each ITTTV element," << std::endl;
	for (auto i : intToTextureTileVector)
	{
		std::cout << "\tsize: " << i->textureVector.size() << std::endl;
	}
}

TextureToTileMapper::~TextureToTileMapper()
{
	for (auto i : intToTextureTileVector)
	{
		delete i;
	}
}

void TextureToTileMapper::drawTile(SDL_Renderer* renderer, int tileIndex, int xPos, int yPos, Uint64 currentTimeInMillis)
{
	intToTextureTileVector[tileIndex]->setRectPos(xPos, yPos);
	SDL_RenderCopy(renderer, intToTextureTileVector[tileIndex]->textureVector[0], NULL, &intToTextureTileVector[tileIndex]->rectangle);
}


	

