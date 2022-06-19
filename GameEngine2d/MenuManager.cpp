
#include "MenuManager.hpp"

MenuManager::MenuManager()
{
    selectedMenuItem = 1;
    titleMenuFont = nullptr;

    menuTitleTextSurfacePtr = nullptr;
    menuTitleTextWrapperPtr = nullptr;
    menuTitleTextTexturePtr = nullptr;
    menuTitleTextWrapper;

    menuStartTextSurfacePtr = nullptr;
    menuStartTextWrapperPtr = nullptr;
    menuStartTextTexturePtr = nullptr;
    menuStartTextWrapper;

    menuStartSelectedTextSurfacePtr = nullptr;
    menuStartSelectedTextWrapperPtr = nullptr;
    menuStartSelectedTextTexturePtr = nullptr;
    menuStartSelectedTextWrapper;

    menuOptionsTextSurfacePtr = nullptr;
    menuOptionsTextWrapperPtr = nullptr;
    menuOptionsTextTexturePtr = nullptr;
    menuOptionsTextWrapper;

    menuOptionsSelectedTextSurfacePtr = nullptr;
    menuOptionsSelectedTextWrapperPtr = nullptr;
    menuOptionsSelectedTextTexturePtr = nullptr;
    menuOptionsSelectedTextWrapper;

    menuQuitTextSurfacePtr = nullptr;
    menuQuitTextWrapperPtr = nullptr;
    menuQuitTextTexturePtr = nullptr;
    menuQuitTextWrapper;

    menuQuitSelectedTextSurfacePtr = nullptr;
    menuQuitSelectedTextWrapperPtr = nullptr;
    menuQuitSelectedTextTexturePtr = nullptr;
    menuQuitSelectedTextWrapper;
}

MenuManager::~MenuManager()
{
    SDL_DestroyTexture(menuTitleTextTexturePtr);
    SDL_DestroyTexture(menuStartTextTexturePtr);
    SDL_DestroyTexture(menuStartSelectedTextTexturePtr);
    SDL_DestroyTexture(menuOptionsTextTexturePtr);
    SDL_DestroyTexture(menuOptionsSelectedTextTexturePtr);
    SDL_DestroyTexture(menuQuitTextTexturePtr);
    SDL_DestroyTexture(menuQuitSelectedTextTexturePtr);
}

int MenuManager::initialize(int resW, int resH, SDL_Renderer* renderer)
{
    if (TTF_Init() == -1) {
        std::cout << "SDL_TTF could not be initialized: " <<
            SDL_GetError() << std::endl;
        return 1;
    }
    else {
        std::cout << "SDL_TTF system ready to go!" << std::endl;
    }

    const std::string gameTitleString = "MOIRÈ";
    titleMenuFont = TTF_OpenFont("./fonts/fette.ttf", 128);

    if (titleMenuFont == nullptr) {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    menuTitleTextWrapper.w = resW / 2.3;
    menuTitleTextWrapper.h = resH / 4.5;
    menuTitleTextWrapper.x = resW / 2.0 - menuTitleTextWrapper.w / 2.0;
    menuTitleTextWrapper.y = menuTitleTextWrapper.h;
    menuTitleTextWrapperPtr = &menuTitleTextWrapper;
    menuTitleTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, gameTitleString.c_str(), { 255, 255, 255 });
    menuTitleTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuTitleTextSurfacePtr);
    SDL_FreeSurface(menuTitleTextSurfacePtr);

    menuStartTextWrapper.w = resW / 4.0 * 5.0 / 7.0;
    menuStartTextWrapper.h = resH / 11.0;
    menuStartTextWrapper.x = (resW - (resW / 4.0 * 5.0 / 7.0)) / 2.0;
    menuStartTextWrapper.y = (resH / 1.6);
    menuStartTextWrapperPtr = &menuStartTextWrapper;
    menuStartTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, "START", { 255, 255, 255 });
    menuStartTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuStartTextSurfacePtr);
    SDL_FreeSurface(menuStartTextSurfacePtr);

    menuStartSelectedTextWrapper.w = resW / 4.0 * 5.0 / 7.0;
    menuStartSelectedTextWrapper.h = resH / 11.0;
    menuStartSelectedTextWrapper.x = (resW - (resW / 4.0 * 5.0 / 7.0)) / 2.0;
    menuStartSelectedTextWrapper.y = resH / 1.6;
    menuStartSelectedTextWrapperPtr = &menuStartSelectedTextWrapper;
    menuStartSelectedTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, "START", { 255, 0, 0 });
    menuStartSelectedTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuStartSelectedTextSurfacePtr);
    SDL_FreeSurface(menuStartSelectedTextSurfacePtr);

    menuOptionsTextWrapper.w = resW / 4.0;
    menuOptionsTextWrapper.h = resH / 11.0;
    menuOptionsTextWrapper.x = (resW - (resW / 4.0)) / 2.0;
    menuOptionsTextWrapper.y = resH / 1.4;
    menuOptionsTextWrapperPtr = &menuOptionsTextWrapper;
    menuOptionsTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, "OPTIONS", { 255, 255, 255 });
    menuOptionsTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuOptionsTextSurfacePtr);
    SDL_FreeSurface(menuOptionsTextSurfacePtr);

    menuOptionsSelectedTextWrapper.w = resW / 4.0;
    menuOptionsSelectedTextWrapper.h = resH / 11.0;
    menuOptionsSelectedTextWrapper.x = (resW - (resW / 4.0)) / 2.0;
    menuOptionsSelectedTextWrapper.y = resH / 1.4;
    menuOptionsSelectedTextWrapperPtr = &menuOptionsSelectedTextWrapper;
    menuOptionsSelectedTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, "OPTIONS", { 255, 0, 0 });
    menuOptionsSelectedTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuOptionsSelectedTextSurfacePtr);
    SDL_FreeSurface(menuOptionsSelectedTextSurfacePtr);

    menuQuitTextWrapper.w = resW / 4.0 * 4.0 / 7.0;
    menuQuitTextWrapper.h = resH / 11.0;
    menuQuitTextWrapper.x = (resW - (resW / 4.0 * 4.0 / 7.0)) / 2.0;
    menuQuitTextWrapper.y = resH / 1.25;
    menuQuitTextWrapperPtr = &menuQuitTextWrapper;
    menuQuitTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, "QUIT", { 255, 255, 255 });
    menuQuitTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuQuitTextSurfacePtr);
    SDL_FreeSurface(menuQuitTextSurfacePtr);

    menuQuitSelectedTextWrapper.w = resW / 4.0 * 4.0 / 7.0;
    menuQuitSelectedTextWrapper.h = resH / 11.0;
    menuQuitSelectedTextWrapper.x = (resW - (resW / 4.0 * 4.0 / 7.0)) / 2.0;
    menuQuitSelectedTextWrapper.y = resH / 1.25;
    menuQuitSelectedTextWrapperPtr = &menuQuitSelectedTextWrapper;
    menuQuitSelectedTextSurfacePtr = TTF_RenderText_Solid(titleMenuFont, "QUIT", { 255, 0, 0 });
    menuQuitSelectedTextTexturePtr = SDL_CreateTextureFromSurface(renderer, menuQuitSelectedTextSurfacePtr);
    SDL_FreeSurface(menuQuitSelectedTextSurfacePtr);
}

void MenuManager::drawMenu(ControlsManager* controlsManager, SDL_Renderer* renderer)
{
    // Title Screen

    SDL_RenderCopy(renderer, menuTitleTextTexturePtr, NULL, menuTitleTextWrapperPtr);

    if (selectedMenuItem == 1)
        SDL_RenderCopy(renderer, menuStartSelectedTextTexturePtr, NULL, menuStartSelectedTextWrapperPtr);
    else SDL_RenderCopy(renderer, menuStartTextTexturePtr, NULL, menuStartTextWrapperPtr);

    if (selectedMenuItem == 2)
        SDL_RenderCopy(renderer, menuOptionsSelectedTextTexturePtr, NULL, menuOptionsSelectedTextWrapperPtr);
    else SDL_RenderCopy(renderer, menuOptionsTextTexturePtr, NULL, menuOptionsTextWrapperPtr);

    if (selectedMenuItem == 3)
        SDL_RenderCopy(renderer, menuQuitSelectedTextTexturePtr, NULL, menuQuitSelectedTextWrapperPtr);
    else SDL_RenderCopy(renderer, menuQuitTextTexturePtr, NULL, menuQuitTextWrapperPtr);


    if (controlsManager->yDir == 1 && SDL_GetTicks64() - controlsManager->lastMenuItemChangeTime > 200)
    {
        selectedMenuItem++;
        controlsManager->lastMenuItemChangeTime = SDL_GetTicks64();
    }

    if (controlsManager->yDir == -1 && SDL_GetTicks64() - controlsManager->lastMenuItemChangeTime > 200)
    {
        selectedMenuItem--;
        controlsManager->lastMenuItemChangeTime = SDL_GetTicks64();
    }

    if (selectedMenuItem == 0)
        selectedMenuItem = 3;

    if (selectedMenuItem == 4)
        selectedMenuItem = 1;
}