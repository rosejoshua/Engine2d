// GameEngine2d.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <math.h>

#include "TextureRectangle.hpp"
#include "TextureTextRectangle.hpp"

int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;
    int resW = 1600;
    int resH = 1024;
    int groundPlane = 885;
    int tileW = 60;

    //Analog joystick dead zone
    const int JOYSTICK_DEAD_ZONE = 5000;
    SDL_Joystick* gGameController = NULL;

    // todo: automate fps to grab the actual setting of refresh rate?
    float fps = 165.0;
    float yVelocity = 0.0;
    float xVelocity = 0.0;

    bool gameIsRunning = true;
    bool showMenu = true;
    int selectedMenuItem = 1;
    bool gameStarted = false;

    //Normalized direction
    int xDir = 0;
    int yDir = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "SDL could not be initialized: " <<
            SDL_GetError();
        return 1;
    }
    else {
        std::cout << "SDL video system is ready to go!" << std::endl;
    }

    //Check for joysticks
    if (SDL_NumJoysticks() < 1)
    {
        std::cout << "Warning: No joysticks connected!" << std::endl;
    }
    else
    {
        //Load joystick
        gGameController = SDL_JoystickOpen(0);
        if (gGameController == NULL)
        {
            std::cout << "Warning: Unable to open game controller! SDL Error: " << std::endl;
        }
    }

    window = SDL_CreateWindow("Pew Pew Pew", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, resW, resH, 0);

    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError();
    }

    static SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (TTF_Init() == -1) {
        std::cout << "SDL_TTF could not be initialized: " <<
            SDL_GetError() << std::endl;
        return 1;
    }
    else {
        std::cout << "SDL_TTF system ready to go!" << std::endl;
    }

    TTF_Font* titleMenuFont = nullptr;
    titleMenuFont = TTF_OpenFont("./fonts/fette.ttf", 32);

    if (titleMenuFont == nullptr) {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    // Texture Asset Creation

    TextureTextRectangle menuTitle = TextureTextRectangle(renderer, "PEW PEW PEW", titleMenuFont, 255, 255, 255);
    menuTitle.SetRectangleProperties(((int)(resW / 1.4)), (resH / 3), ((int)(resW - (int)(resW / 1.4)) / 2), (resH / 8));

    TextureTextRectangle menuStart = TextureTextRectangle(renderer, "Start", titleMenuFont, 255, 255, 255);
    menuStart.SetRectangleProperties(((int)(resW / 4.0 * 5.0 / 7.0)), (resH / 9), ((int)(resW - (int)(resW / 4.0 * 5.0 / 7.0)) / 2), (resH / 1.6));
    TextureTextRectangle menuStartSelected = TextureTextRectangle(renderer, "Start", titleMenuFont, 255, 0, 0);
    menuStartSelected.SetRectangleProperties(((int)(resW / 4.0 * 5.0 / 7.0)), (resH / 9), ((int)(resW - (int)(resW / 4.0 * 5.0 / 7.0)) / 2), (resH / 1.6));

    TextureTextRectangle menuOptions = TextureTextRectangle(renderer, "Options", titleMenuFont, 255, 255, 255);
    menuOptions.SetRectangleProperties(((int)(resW / 4.0)), (resH / 9), ((int)(resW - (int)(resW / 4.0)) / 2), (resH / 1.4));
    TextureTextRectangle menuOptionsSelected = TextureTextRectangle(renderer, "Options", titleMenuFont, 255, 0, 0);
    menuOptionsSelected.SetRectangleProperties(((int)(resW / 4.0)), (resH / 9), ((int)(resW - (int)(resW / 4.0)) / 2), (resH / 1.4));

    TextureTextRectangle menuQuit = TextureTextRectangle(renderer, "Quit", titleMenuFont, 255, 255, 255);
    menuQuit.SetRectangleProperties(((int)(resW / 4.0 * 4.0 / 7.0)), (resH / 9), ((int)(resW - (int)(resW / 4.0 * 4.0 / 7.0)) / 2), (resH / 1.23));
    TextureTextRectangle menuQuitSelected = TextureTextRectangle(renderer, "Quit", titleMenuFont, 255, 0, 0);
    menuQuitSelected.SetRectangleProperties(((int)(resW / 4.0 * 4.0 / 7.0)), (resH / 9), ((int)(resW - (int)(resW / 4.0 * 4.0 / 7.0)) / 2), (resH / 1.23));

    //TextureRectangle textureBackground = TextureRectangle(renderer, "./images/TestBackground.bmp");
    //textureBackground.SetRectangleProperties(3200, 1024, 0, 0);

    TextureRectangle textureBackgroundTile = TextureRectangle(renderer, "./images/TestBackgroundTile.bmp");

    // Create a rectangle for player model
    SDL_Rect playerRect;
    playerRect.w = 40;
    playerRect.h = 80;
    playerRect.x = 610;
    playerRect.y = 30;

    // Infinite loop for our application

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
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
                selectedMenuItem--;

                if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
                    selectedMenuItem++;

                if (selectedMenuItem == 0)
                    selectedMenuItem = 3;

                if (selectedMenuItem == 4)
                    selectedMenuItem = 1;

                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2) {
                    if (selectedMenuItem == 1) {
                        showMenu = false;
                        gameStarted = true;
                    }
                    else if (selectedMenuItem == 2)
                        std::cout << "Options Menu Selected Without Implementation" << std::endl;
                    else if (selectedMenuItem == 3)
                        gameIsRunning = false;
                }

                if (event.key.keysym.sym == SDLK_ESCAPE && gameStarted) {
                    showMenu = true;
                    gameStarted = false;
                }

                if (event.key.keysym.sym == SDLK_SPACE) {
                    yVelocity -= 16;
                    playerRect.y += (int)yVelocity;
                }
            }
            else if (event.type == SDL_JOYAXISMOTION)
            {
                //Motion on controller 0
                if (event.jaxis.which == 0)
                {
                    //X axis motion
                    if (event.jaxis.axis == 0)
                    {
                        //Left of dead zone
                        if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
                        {
                            xDir = -1;
                        }
                        //Right of dead zone
                        else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
                        {
                            xDir = 1;
                        }
                        else
                        {
                            xDir = 0;
                        }
                    }
                    //Y axis motion
                    else if (event.jaxis.axis == 1)
                    {
                        //Below of dead zone
                        if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
                        {
                            yDir = -1;
                        }
                        //Above of dead zone
                        else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
                        {
                            yDir = 1;
                        }
                        else
                        {
                            yDir = 0;
                        }
                    }
                }
            }
        }
        // (2) Handle Updates

        // Apply gravity to vertical velocity if airborne, seems reversed because 
        // grid origin is top left of screen...

        if (playerRect.y <= (groundPlane - 80) && gameStarted) {
            yVelocity += (80.0 / fps);
            playerRect.y += (int)yVelocity;

            if (playerRect.y > (groundPlane - 80)) {
                playerRect.y = groundPlane - 80;
                yVelocity = 0.0;
            }
        }

        if (xDir == -1) {
            if (xVelocity >= -20.0) {
                xVelocity -= (15.0 / fps);
                if (xVelocity < -20.0) {
                    xVelocity = -20.0;
                }
                playerRect.x += (int)xVelocity;
            }
        }
        else if (xDir == 1) {
            if (xVelocity <= 20.0) {
                xVelocity += (15.0 / fps);
                if (xVelocity > 20.0) {
                    xVelocity = 20.0;
                }
                playerRect.x += (int)xVelocity;
            }
        }
        else xVelocity = 0.0;

        // todo: need check here to make sure engine isn't lagging
        // using SDL_GetTicks
        SDL_Delay((int)(1000.0/fps));

        // (3) Clear and Draw the Screen
        // Gives us a clear "canvas"
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (showMenu) {
        // Title Screen
           
            menuTitle.render(renderer);
            
            selectedMenuItem == 1 ? menuStartSelected.render(renderer) : menuStart.render(renderer);
            selectedMenuItem == 2 ? menuOptionsSelected.render(renderer) : menuOptions.render(renderer);
            selectedMenuItem == 3 ? menuQuitSelected.render(renderer) : menuQuit.render(renderer);
        }

        if (gameStarted) {
            //textureBackground.render(renderer);
            
        for (unsigned char i = 0; i <= resW / tileW; i++)
        {
            for (unsigned char j = 0; j <= resH / tileW; j++) 
            {
                textureBackgroundTile.SetRectangleProperties( tileW, tileW, i * tileW, j * tileW );
                textureBackgroundTile.render(renderer);
            }
        }

            SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
            SDL_RenderFillRect(renderer, &playerRect);
        }

        // Finally show what we've drawn
        SDL_RenderPresent(renderer);
    }

    //Close game controller
    SDL_JoystickClose(gGameController);
    gGameController = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
