// C++ Standard Libraries
#include <string>
// Third-party library
#include "SDL.h"
// Local library
#include "TextureRectangle.hpp"

    TextureRectangle::TextureRectangle(SDL_Renderer*& renderer, std::string filepath) {
        SDL_Surface* surface = SDL_LoadBMP(filepath.c_str());
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    TextureRectangle::~TextureRectangle() {
        SDL_DestroyTexture(m_texture);
    }

    void TextureRectangle::SetRectangleProperties(int w, int h, int x, int y) {
        m_rectangle.w = w;
        m_rectangle.h = h;
        m_rectangle.x = x;
        m_rectangle.y = y;
    }

    void TextureRectangle::render(SDL_Renderer*& renderer) {
        SDL_RenderCopy(renderer, m_texture, NULL, &m_rectangle);
    }