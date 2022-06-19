#pragma once
#include<iostream>
#include<string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "ControlsManager.hpp"

class MenuManager
{
public:
    int selectedMenuItem;
    MenuManager();
    ~MenuManager();
	TTF_Font* titleMenuFont;

    SDL_Surface* menuTitleTextSurfacePtr;
    SDL_Rect* menuTitleTextWrapperPtr;
    SDL_Texture* menuTitleTextTexturePtr;
    SDL_Rect menuTitleTextWrapper;

    SDL_Surface* menuStartTextSurfacePtr;
    SDL_Rect* menuStartTextWrapperPtr;
    SDL_Texture* menuStartTextTexturePtr;
    SDL_Rect menuStartTextWrapper;

    SDL_Surface* menuStartSelectedTextSurfacePtr;
    SDL_Rect* menuStartSelectedTextWrapperPtr;
    SDL_Texture* menuStartSelectedTextTexturePtr;
    SDL_Rect menuStartSelectedTextWrapper;

    SDL_Surface* menuOptionsTextSurfacePtr;
    SDL_Rect* menuOptionsTextWrapperPtr;
    SDL_Texture* menuOptionsTextTexturePtr;
    SDL_Rect menuOptionsTextWrapper;

    SDL_Surface* menuOptionsSelectedTextSurfacePtr;
    SDL_Rect* menuOptionsSelectedTextWrapperPtr;
    SDL_Texture* menuOptionsSelectedTextTexturePtr;
    SDL_Rect menuOptionsSelectedTextWrapper;

    SDL_Surface* menuQuitTextSurfacePtr;
    SDL_Rect* menuQuitTextWrapperPtr;
    SDL_Texture* menuQuitTextTexturePtr;
    SDL_Rect menuQuitTextWrapper;

    SDL_Surface* menuQuitSelectedTextSurfacePtr;
    SDL_Rect* menuQuitSelectedTextWrapperPtr;
    SDL_Texture* menuQuitSelectedTextTexturePtr;
    SDL_Rect menuQuitSelectedTextWrapper;

    int initialize(int resW, int resH, SDL_Renderer* renderer);

    void drawMenu(ControlsManager* controlsManager, SDL_Renderer* renderer);
};

