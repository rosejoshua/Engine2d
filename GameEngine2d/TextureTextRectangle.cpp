// C++ Standard Libraries
#include <string>

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"

// local library
#include "TextureTextRectangle.hpp"

TextureTextRectangle::TextureTextRectangle(SDL_Renderer*& renderer, std::string textToDisplay, TTF_Font*& font, int red, int green, int blue) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textToDisplay.c_str(), { (unsigned char)red, (unsigned char)green, (unsigned char)blue });
	m_textureText = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
}

TextureTextRectangle::~TextureTextRectangle() {
	SDL_DestroyTexture(m_textureText);
}

void TextureTextRectangle::SetRectangleProperties(int w, int h, int x, int y) {
    m_textWrapper.w = w;
    m_textWrapper.h = h;
    m_textWrapper.x = x;
    m_textWrapper.y = y;
}

void TextureTextRectangle::render(SDL_Renderer*& renderer) {
    SDL_RenderCopy(renderer, m_textureText, NULL, &m_textWrapper);
}