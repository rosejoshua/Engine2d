#pragma once

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"

// C++ Standard Libraries
#include <string>

class TextureTextRectangle
{
public:
	SDL_Rect m_textWrapper;
	SDL_Texture* m_textureText;

	TextureTextRectangle(SDL_Renderer*& renderer, std::string textToDisplay, TTF_Font*& font, int red, int green, int blue);
	~TextureTextRectangle();

	void SetRectangleProperties(int x, int y, int w, int h);

	void render(SDL_Renderer*& renderer);
};

