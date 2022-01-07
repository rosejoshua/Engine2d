// GameEngine2d.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"

// C++ Standard Libraries
#include <iostream>


int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;
    unsigned short int resW = 1280;
    unsigned short int resH = 1024;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized: " <<
            SDL_GetError();
        return 1;
    }
    else {
        std::cout << "SDL video system is ready to go!" << std::endl;
    }

    window = SDL_CreateWindow("American Jo", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, resW, resH, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError();
    }

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (TTF_Init() == -1) {
        std::cout << "SDL_TTF could not be initialized: " <<
            SDL_GetError() << std::endl;
        return 1;
    }
    else {
        std::cout << "SDL_TTF system ready to go!" << std::endl;
    }

    TTF_Font* titleMenuFont = TTF_OpenFont("./fonts/fette.ttf", 32);
    if (titleMenuFont == nullptr) {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    SDL_Surface* surfaceTextTitle = TTF_RenderText_Solid(titleMenuFont, "AMERICAN JO", { 255,255,255 });
    SDL_Texture* textureTextTitle = SDL_CreateTextureFromSurface(renderer, surfaceTextTitle);
    // Free the surface
    SDL_FreeSurface(surfaceTextTitle);

    SDL_Surface* surfaceTextStart = TTF_RenderText_Solid(titleMenuFont, "Start", { 255,255,255 });
    SDL_Texture* textureTextStart = SDL_CreateTextureFromSurface(renderer, surfaceTextStart);
    // Free the surface
    SDL_FreeSurface(surfaceTextStart);

    SDL_Surface* surfaceTextOptions = TTF_RenderText_Solid(titleMenuFont, "Options", { 255,255,255 });
    SDL_Texture* textureTextOptions = SDL_CreateTextureFromSurface(renderer, surfaceTextOptions);
    // Free the surface
    SDL_FreeSurface(surfaceTextOptions);

    SDL_Surface* surfaceTextQuit = TTF_RenderText_Solid(titleMenuFont, "Quit", { 255,255,255 });
    SDL_Texture* textureTextQuit = SDL_CreateTextureFromSurface(renderer, surfaceTextQuit);
    // Free the surface
    SDL_FreeSurface(surfaceTextQuit);


    // Create a rectangle for Title
    SDL_Rect titleWrapper;
    titleWrapper.w = (int)(resW / 1.4);
    titleWrapper.h = resH / 3;
    titleWrapper.x = (int)(resW - titleWrapper.w) / 2;
    titleWrapper.y = resH / 8;

    //Create 3 rectangles for Menu Items
    SDL_Rect startWrapper;
    startWrapper.w = (int)(resW / 4.0 * 5.0 / 7.0);
    startWrapper.h = resH / 9;
    startWrapper.x = (int)(resW - startWrapper.w) / 2;
    startWrapper.y = resH / 1.6;

    SDL_Rect optionsWrapper;
    optionsWrapper.w = (int)(resW / 4.0);
    optionsWrapper.h = resH / 9;
    optionsWrapper.x = (int)(resW - optionsWrapper.w) / 2;
    optionsWrapper.y = resH / 1.4;

    SDL_Rect quitWrapper;
    quitWrapper.w = (int)(resW / 4.0 * 4.0 / 7.0);
    quitWrapper.h = resH / 9;
    quitWrapper.x = (int)(resW - quitWrapper.w) / 2;
    quitWrapper.y = resH / 1.23;

    // Infinite loop for our application
    bool gameIsRunning = true;
    bool showMenu = true;
    // Main application loop
    while (gameIsRunning) {
        SDL_Event event;

        // (1) Handle Input
        // Start our event loop
        while (SDL_PollEvent(&event)) {
            // Handle each specific event
            if (event.type == SDL_QUIT) {
                gameIsRunning = false;
            }
        }
        // (2) Handle Updates

        // (3) Clear and Draw the Screen
        // Gives us a clear "canvas"
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (showMenu) {
        // Title Screen
        SDL_RenderCopy(renderer, textureTextTitle, NULL, &titleWrapper);
        SDL_RenderCopy(renderer, textureTextStart, NULL, &startWrapper);
        SDL_RenderCopy(renderer, textureTextOptions, NULL, &optionsWrapper);
        SDL_RenderCopy(renderer, textureTextQuit, NULL, &quitWrapper);
        }

        // Do our drawing
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        //SDL_RenderDrawLine(renderer, 5, 5, 100, 120);

        //SDL_RenderDrawRect(renderer,&rectangle);
        //SDL_RenderCopy(renderer, texture, NULL, &rectangle);


        // Finally show what we've drawn
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(textureTextTitle);
    SDL_DestroyTexture(textureTextStart);
    SDL_DestroyTexture(textureTextOptions);
    SDL_DestroyTexture(textureTextQuit);
    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the 
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
