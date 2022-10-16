#pragma once

#include<iostream>
#include<string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "ControlsManager.hpp"
#include "MyRGB.hpp"
#include "SDL_mixer.h"

class YouDiedManager
{
public:
	YouDiedManager();
	~YouDiedManager();

    const std::string youDiedString = "YOU DIED";
    bool soundEffectStarted;

    MyRGB deadBackgroundColor;

    TTF_Font* youDiedFont;

    SDL_Surface* youDiedTextSurfacePtr;
    SDL_Rect* youDiedTextWrapperPtr;
    SDL_Texture* youDiedTextTexturePtr;
    SDL_Rect youDiedTextWrapper;

    int initialize(int resW, int resH, SDL_Renderer* renderer);

    void drawYouDied(SDL_Renderer* renderer, bool* dead, Uint64 diedAt, bool* gameStarted, bool* showMenu, int resW, int resH, Mix_Chunk* youDiedSound);
};

