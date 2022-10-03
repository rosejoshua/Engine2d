#include "TextureToTileMapper.hpp"

TextureToTileMapper::TextureToTileMapper(SDL_Renderer*& renderer, int tileW)
{
	int fileIndex = 0;
	int animationIndex = 0;
	std::string tileBehaviorFilename = "tileBehaviorInput.txt";
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
				SDL_SetColorKey(tempSurfacePointer, SDL_TRUE, SDL_MapRGB(tempSurfacePointer->format, 255, 255, 255));
				textureTilePtr->textureVector.push_back(SDL_CreateTextureFromSurface(renderer, tempSurfacePointer));
				SDL_FreeSurface(tempSurfacePointer);
				animationIndex++;
				tempSurfacePointer = SDL_LoadBMP((imagesDir + std::to_string(fileIndex) + "/" +
					std::to_string(tileW) + "-" + std::to_string(animationIndex) + ".bmp").c_str());
			}
			animationIndex = 0;
			//grab tile characteristics from text file in images folder
			std::ifstream file;
			file.open(imagesDir + std::to_string(fileIndex) + "/" + tileBehaviorFilename);
			if (!file)
			{
				std::cerr << "failure to open " << tileBehaviorFilename << ", tile behaviors will not be imported!" << std::endl;;
			}
			else 
			{
				std::string tempString = "";
				bool tempBool = false;
				int tempInt = 0;

				//ignoring 2 lines of instructions/comments from tileBehaviorInput.txt
				std::getline(file, tempString);
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->isCollision = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->drawsInFront = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->isLethal = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->hasSpecialCharacteristics = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->isAnimated = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->isConsumable = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->isLiquid = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->hasCollisionAnimation = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);

				textureTilePtr->hasTransformation = (tempString == "true");
				std::getline(file, tempString);
				std::getline(file, tempString);
				tempInt = std::stoi(tempString);

				textureTilePtr->tileTransformationBehaviorIndex = tempInt;
				std::getline(file, tempString);
				std::getline(file, tempString);
				tempInt = std::stoi(tempString);

				textureTilePtr->minimumCollisionRatio = tempInt;
				std::getline(file, tempString);
				std::getline(file, tempString);
				tempInt = std::stoi(tempString);

				textureTilePtr->transformationTileIndex = tempInt;
				std::getline(file, tempString);
				std::getline(file, tempString);
				tempInt = std::stoi(tempString);

				textureTilePtr->transparencyAlphaValue = tempInt;
				std::getline(file, tempString);
				std::getline(file, tempString);
				tempInt = std::stoi(tempString);

				textureTilePtr->desiredMillisBetweenAnimationFrames = tempInt;

				file.close();

				intToTextureTileVector.push_back(textureTilePtr);
				textureTilePtr = new TextureTile(tileW);
			}

		}
		
	}
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
	if (intToTextureTileVector[tileIndex]->isAnimated)
	{
		SDL_RenderCopy(renderer, intToTextureTileVector[tileIndex]->textureVector[
			
			//get animation index using timestamp
			(SDL_GetTicks64() % (intToTextureTileVector[tileIndex]->textureVector.size() * intToTextureTileVector[tileIndex]->desiredMillisBetweenAnimationFrames)) 

				/
				
				(intToTextureTileVector[tileIndex]->desiredMillisBetweenAnimationFrames)


		], NULL, &intToTextureTileVector[tileIndex]->rectangle);
	}
	else
	{
		SDL_RenderCopy(renderer, intToTextureTileVector[tileIndex]->textureVector[0], NULL, &intToTextureTileVector[tileIndex]->rectangle);
	}
}


	

