#include "YouDiedManager.hpp"

YouDiedManager::YouDiedManager()
{
    soundEffectStarted = false;
    youDiedFont = nullptr;

	youDiedTextSurfacePtr = nullptr;
	youDiedTextWrapperPtr = nullptr;
	youDiedTextTexturePtr = nullptr;
	youDiedTextWrapper;
}

YouDiedManager::~YouDiedManager()
{
	SDL_DestroyTexture(youDiedTextTexturePtr);
}

int YouDiedManager::initialize(int resW, int resH, SDL_Renderer* renderer)
{
    if (TTF_Init() == -1) 
    {
        std::cout << "SDL_TTF could not be initialized: " <<
            SDL_GetError() << std::endl;
        return 1;
    }
    else 
    {
        std::cout << "SDL_TTF system ready to go for YouDiedManager!" << std::endl;
    }

    youDiedFont = TTF_OpenFont("./fonts/8bitOperatorPlus-Regular.ttf", resW / 3 / youDiedString.size());

    if (youDiedFont == nullptr) 
    {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    youDiedTextWrapper.w = resW / 2;
    youDiedTextWrapper.h = youDiedTextWrapper.w / youDiedString.size() *2;
    youDiedTextWrapper.x = resW / 2.0 - youDiedTextWrapper.w / 2.0;
    youDiedTextWrapper.y = resH / 2.0 - youDiedTextWrapper.h / 2.0;
    youDiedTextWrapperPtr = &youDiedTextWrapper;
    youDiedTextSurfacePtr = TTF_RenderText_Solid(youDiedFont, youDiedString.c_str(), { 150, 7, 6 });
    youDiedTextTexturePtr = SDL_CreateTextureFromSurface(renderer, youDiedTextSurfacePtr);
    SDL_SetTextureBlendMode(youDiedTextTexturePtr, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(youDiedTextTexturePtr, 0);
    SDL_FreeSurface(youDiedTextSurfacePtr);
}

void YouDiedManager::drawYouDied(SDL_Renderer* renderer, bool* dead, Uint64 diedAt, bool* gameStarted, bool* showMenu, int resW, int resH/*, Sound* youDiedSound*/)
{

    if (!soundEffectStarted)
    {
        //todo:play async using callback eventually
        //youDiedSound->PlaySound();
        //soundEffectStarted = true;
    }
    //4 seconds of YouDied screen
    if (SDL_GetTicks64() < diedAt + 4001 )
    {
        SDL_SetRenderDrawColor(renderer, deadBackgroundColor.r, deadBackgroundColor.g, deadBackgroundColor.b, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        //delay fade in by a second
        if (SDL_GetTicks64() - diedAt > 500)
        {
            
            youDiedTextWrapper.w = (resW / 2) + ((SDL_GetTicks64() - diedAt - 500) * 150) / 3500;
            youDiedTextWrapper.h = youDiedTextWrapper.w / youDiedString.size() * 2;
            youDiedTextWrapper.x = resW / 2.0 - youDiedTextWrapper.w / 2.0;
            youDiedTextWrapper.y = resH / 2.0 - youDiedTextWrapper.h / 2.0;

            //3 seconds of fade in
            //clamping necessary to have letters completely fade in while still growing and screen continuing
            SDL_SetTextureAlphaMod(youDiedTextTexturePtr, ((((SDL_GetTicks64() - diedAt - 500 ) * 255) / 3000) > 255) ?
                255
                :
                ((SDL_GetTicks64() - diedAt - 500) * 255) / 3000);
        }

        SDL_RenderCopy(renderer, youDiedTextTexturePtr, NULL, youDiedTextWrapperPtr);
    }
    else
    {
        *dead = false;
        *gameStarted = false;
        *showMenu = true;
        soundEffectStarted = false;

        youDiedTextWrapper.w = resW / 2;
        youDiedTextWrapper.h = youDiedTextWrapper.w / youDiedString.size() * 2;
        youDiedTextWrapper.x = resW / 2.0 - youDiedTextWrapper.w / 2.0;
        youDiedTextWrapper.y = resH / 2.0 - youDiedTextWrapper.h / 2.0;
        SDL_SetTextureAlphaMod(youDiedTextTexturePtr, 0);
    }
    
}
