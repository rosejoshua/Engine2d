#include "ScoreManager.hpp"

ScoreManager::ScoreManager()
{
    score = 0;
    highScore = 0;
    previousHorizontalProgress = 0;
    scoreString = std::to_string(score);
    highScoreString = std::to_string(highScore);
    multiplier = 0.001;

    scoreFont = nullptr;

    scoreTextSurfacePtr = nullptr;
    scoreTextWrapperPtr = nullptr;
    scoreTextTexturePtr = nullptr;
    scoreTextWrapper;

    highScoreTextSurfacePtr = nullptr;
    highScoreTextWrapperPtr = nullptr;
    highScoreTextTexturePtr = nullptr;
    highScoreTextWrapper;
}

ScoreManager::~ScoreManager()
{
    SDL_DestroyTexture(scoreTextTexturePtr);
    SDL_DestroyTexture(highScoreTextTexturePtr);
}

void ScoreManager::padScores()
{
    scoreString = std::string(NUM_SCORE_SPACES - scoreString.length(), ' ') + scoreString;
    highScoreString = std::string(NUM_SCORE_SPACES - highScoreString.length(), ' ') + highScoreString;
}

int ScoreManager::initialize(int resW, int resH, SDL_Renderer* renderer)
{
    if (TTF_Init() == -1) {
        std::cout << "SDL_TTF could not be initialized: " <<
            SDL_GetError() << std::endl;
        return 1;
    }
    else {
        std::cout << "SDL_TTF system ready to go!" << std::endl;
    }

    scoreFont = TTF_OpenFont("./fonts/fette.ttf", 32);

    if (scoreFont == nullptr) {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }
    padScores();
    scoreTextWrapper.w = resW / 4;
    scoreTextWrapper.h = resH / 20;
    scoreTextWrapper.x = resW / 100;
    scoreTextWrapper.y = resH / 100;
    scoreTextWrapperPtr = &scoreTextWrapper;
    scoreTextSurfacePtr = TTF_RenderText_Solid(scoreFont, scoreString.c_str(), { 255, 255, 255 });
    scoreTextTexturePtr = SDL_CreateTextureFromSurface(renderer, scoreTextSurfacePtr);
    SDL_FreeSurface(scoreTextSurfacePtr);

    highScoreTextWrapper.w = resW / 4;
    highScoreTextWrapper.h = resH / 20;
    highScoreTextWrapper.x = resW / 4 * 3 - resW / 100;
    highScoreTextWrapper.y = resH / 100;
    highScoreTextWrapperPtr = &highScoreTextWrapper;
    highScoreTextSurfacePtr = TTF_RenderText_Solid(scoreFont, highScoreString.c_str(), { 255, 255, 255 });
    highScoreTextTexturePtr = SDL_CreateTextureFromSurface(renderer, highScoreTextSurfacePtr);
    SDL_FreeSurface(highScoreTextSurfacePtr);
}

void ScoreManager::drawScore(SDL_Renderer* renderer)
{

    SDL_DestroyTexture(scoreTextTexturePtr);
    scoreTextTexturePtr;
    scoreString = std::to_string(score);
    padScores();
    scoreTextSurfacePtr = TTF_RenderText_Solid(scoreFont, scoreString.c_str(), { 255, 255, 255 });
    scoreTextTexturePtr = SDL_CreateTextureFromSurface(renderer, scoreTextSurfacePtr);
    SDL_FreeSurface(scoreTextSurfacePtr);
    SDL_RenderCopy(renderer, scoreTextTexturePtr, NULL, scoreTextWrapperPtr);
    SDL_RenderCopy(renderer, highScoreTextTexturePtr, NULL, highScoreTextWrapperPtr);
}

void ScoreManager::updateScoreByProgress(Uint64 horizontalProgress)
{
    score += (horizontalProgress - previousHorizontalProgress) * (multiplier * SCORE_BASE);
    previousHorizontalProgress = horizontalProgress;
}

void ScoreManager::addConsumablePoints(int tileId)
{
    if (tileId == 2)
        score += 100;
}
