#pragma once

// C++ Standard Libraries
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// Third-party library
#include "SDL.h"

// Local library
#include "TextureTile.hpp"

class TextureToTileMapper
{
public:
	const std::string imagesDir = "./images/";

	TextureToTileMapper(SDL_Renderer*& renderer, int tileW);
	~TextureToTileMapper();

	void drawTile(SDL_Renderer* renderer, int tileIndex, int xPos, int yPos, Uint64 currentTimeInMillis);

	std::vector <TextureTile*> intToTextureTileVector;
};
