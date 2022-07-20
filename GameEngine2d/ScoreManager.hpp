#pragma once

#include<iostream>
#include<string>
#include "SDL.h"
#include "SDL_ttf.h"

class ScoreManager
{
public:
    ScoreManager();
    ~ScoreManager();

    TTF_Font* scoreFont;

    SDL_Surface* scoreTextSurfacePtr;
    SDL_Rect* scoreTextWrapperPtr;
    SDL_Texture* scoreTextTexturePtr;
    SDL_Rect scoreTextWrapper;

    SDL_Surface* highScoreTextSurfacePtr;
    SDL_Rect* highScoreTextWrapperPtr;
    SDL_Texture* highScoreTextTexturePtr;
    SDL_Rect highScoreTextWrapper;

    const int SCORE_BASE = 1000;
    const int NUM_SCORE_SPACES = 20;
    Uint64 score;
    Uint64 highScore;
    Uint64 previousHorizontalProgress;
    double multiplier;
    std::string scoreString;
    std::string highScoreString;

    int initialize(int resW, int resH, SDL_Renderer* renderer);
    void drawScore(SDL_Renderer* renderer);
    void updateScoreByProgress(Uint64 horizontalProgress);
    void padScores();
};

