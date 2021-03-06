#pragma once

// Third-party library
#include "SDL.h"

// C++ Standard Libraries
#include <string>

class TextureRectangle {
public:
    SDL_Rect m_rectangle;
    SDL_Texture* m_texture;

    TextureRectangle(SDL_Renderer*& renderer, std::string filepath);
    ~TextureRectangle();

    void setRectangleProperties(int w, int h, int x, int y);

    void render(SDL_Renderer*& renderer);
};