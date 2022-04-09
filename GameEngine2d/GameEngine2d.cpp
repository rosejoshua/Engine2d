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
#include "TestMap.hpp"

int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;
    int resW = 2000;
    int resH = 1200;
    int tileW = 60;
    int playerHeight = tileW*2.5;
    int playerWidth = tileW*1.5;

    int cameraX = 0;
    int cameraY = 0;

    const float jumpVelocity = playerHeight/-40.0;
    //const float horVelModPlayer = 0.5;
    const float horVelModPlayerSprint = playerHeight/600.0;
    //gravity per tick
    const float gravityModifier = playerHeight/9000.0;
    const float heavyGravityModifier = 0.4;
    const int maxWalkVelocity = playerHeight/12;
    const int maxHorSprintVelocity = playerHeight/75;

    Uint64 lastPhysicsUpdate = SDL_GetTicks64();

    TestMap* testMap = new TestMap();
    
    //Analog joystick dead zone
    const int JOYSTICK_DEAD_ZONE = 5000;
    SDL_Joystick* gGameController = NULL;
    float yVelocity = 0.0;
    float xVelocity = 0.0;

    bool gameIsRunning = true;
    bool showMenu = true;
    int selectedMenuItem = 1;
    bool gameStarted = false;

    //joy direction reduced to int from -1 to 1
    int xDir = 0;
    int xDirLast = 0;
    int yDir = 0;

    int tilePastPlayerRect = 0;

    bool button0Down = false;
    bool button1Down = false;
    bool button2Down = false;
    bool button3Down = false;
    bool button4Down = false;
    bool button5Down = false;
    bool button6Down = false;
    bool button7Down = false;
    bool button8Down = false;
    bool button9Down = false;
    bool button10Down = false;
    bool button11Down = false;
    bool button12Down = false;

    bool canJump = true;

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

    TextureRectangle textureBackgroundSky = TextureRectangle(renderer, "./images/TestBackgroundTile.bmp");
    TextureRectangle textureGround = TextureRectangle(renderer, "./images/ground.bmp");
    TextureRectangle textureSpikes = TextureRectangle(renderer, "./images/spikes.bmp");

    // Create a rectangle for player model
    SDL_Rect playerRect;
    playerRect.w = playerWidth;
    playerRect.h = playerHeight;
    playerRect.x = resW/3 - playerWidth/2;
    playerRect.y = 0;

    //Create a second rectangle for player model previous position, necessary for backtracking player position in the event of a collision
    SDL_Rect prevPlayerRect;
    prevPlayerRect.w = playerRect.w;
    prevPlayerRect.h = playerRect.h;
    prevPlayerRect.x = playerRect.x;
    prevPlayerRect.y = playerRect.y;

    //Create a rect to hold dimensions and position of a map tile for collision modeling
    SDL_Rect tileCollisionRect;
    tileCollisionRect.w = tileW;
    tileCollisionRect.h = tileW;
    tileCollisionRect.x = 0;
    tileCollisionRect.y = 0;

    //Create a rectangle for union of playerRect and tileRect modeling collision overlap
    SDL_Rect collisionIntersectionRect;
    collisionIntersectionRect.x = 0;
    collisionIntersectionRect.y = 0;
    collisionIntersectionRect.w = 0;
    collisionIntersectionRect.h = 0;

    //Create a rectangle strictly for drawing
    SDL_Rect playerDrawingRect;
    playerDrawingRect.x = playerRect.x - cameraX;
    playerDrawingRect.y = playerRect.y - cameraY;
    playerDrawingRect.w = playerRect.w;
    playerDrawingRect.h = playerRect.h;

    // Infinite loop for our application

    // Main application loop
    while (gameIsRunning) 
    {   
        while (SDL_GetTicks64() - lastPhysicsUpdate < 6) {}
        SDL_Event event;

        // (1) Handle Input
        // Start our event loop
        while (SDL_PollEvent(&event)) 
        {
            // Handle each specific event
            if (event.type == SDL_QUIT) {
                gameIsRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) 
            {
                if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
                selectedMenuItem--;

                if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
                    selectedMenuItem++;

                if (selectedMenuItem == 0)
                    selectedMenuItem = 3;

                if (selectedMenuItem == 4)
                    selectedMenuItem = 1;

                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2) 
                {
                    if (selectedMenuItem == 1) {
                        showMenu = false;
                        gameStarted = true;
                    }
                    else if (selectedMenuItem == 2)
                        std::cout << "Options Menu Selected Without Implementation" << std::endl;
                    else if (selectedMenuItem == 3)
                        gameIsRunning = false;
                }

                if (event.key.keysym.sym == SDLK_ESCAPE && gameStarted) 
                {
                    showMenu = true;
                    gameStarted = false;
                }

                if (event.key.keysym.sym == SDLK_SPACE) 
                {
                    yVelocity += jumpVelocity;
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
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                if (event.jbutton.button == 0) {
                    button0Down = true;
                }
                else if (event.jbutton.button == 1) {
                    button1Down = true;
                }
                else if (event.jbutton.button == 2) {
                    button2Down = true;
                }
                else if (event.jbutton.button == 3) {
                    button3Down = true;
                }
                else if (event.jbutton.button == 4) {
                    button4Down = true;
                }
                else if (event.jbutton.button == 5) {
                    button5Down = true;
                }
                else if (event.jbutton.button == 6) {
                    button6Down = true;
                }
                else if (event.jbutton.button == 7) {
                    button7Down = true;
                }
                else if (event.jbutton.button == 8) {
                    button8Down = true;
                }
                else if (event.jbutton.button == 9) {
                    button9Down = true;
                }
                else if (event.jbutton.button == 10) {
                    button10Down = true;
                }
                else if (event.jbutton.button == 11) {
                    button11Down = true;
                }
                else if (event.jbutton.button == 12) {
                    button12Down = true;
                }
            }
            else if (event.type == SDL_JOYBUTTONUP)
            {
                if (event.jbutton.button == 0) {
                    button0Down = false;
                }
                else if (event.jbutton.button == 1) {
                    button1Down = false;
                }
                else if (event.jbutton.button == 2) {
                    button2Down = false;
                }
                else if (event.jbutton.button == 3) {
                    button3Down = false;
                }
                else if (event.jbutton.button == 4) {
                    button4Down = false;
                }
                else if (event.jbutton.button == 5) {
                    button5Down = false;
                }
                else if (event.jbutton.button == 6) {
                    button6Down = false;
                }
                else if (event.jbutton.button == 7) {
                    button7Down = false;
                }
                else if (event.jbutton.button == 8) {
                    button8Down = false;
                }
                else if (event.jbutton.button == 9) {
                    button9Down = false;
                }
                else if (event.jbutton.button == 10) {
                    button10Down = false;
                }
                else if (event.jbutton.button == 11) {
                    button11Down = false;
                }
                else if (event.jbutton.button == 12) {
                    button12Down = false;
                }
            }

        }

        

        // (2) Handle Updates
        // grid origin is top left of screen...
        
        //add jump to velocity
        if (button0Down && canJump) {
            yVelocity += jumpVelocity;
            canJump = false;
        }

        //move player position on x axis based on xVelocity
        
        if (xDir == -1 && xDirLast != 1) {
            if (xVelocity > -maxHorSprintVelocity) {
                xVelocity -= horVelModPlayerSprint;
                if (xVelocity < -maxHorSprintVelocity) {
                    xVelocity = -maxHorSprintVelocity;
                }
            }
        }
        else if (xDir == 1 && xDirLast != -1) {
            if (xVelocity <= maxHorSprintVelocity) {
                xVelocity += horVelModPlayerSprint;
                if (xVelocity > maxHorSprintVelocity) {
                    xVelocity = maxHorSprintVelocity;
                }
            }
        }
        else {
            xVelocity = 0.0;
        }
        

        if (gameStarted) {

            //capture playerPosition
            prevPlayerRect.x = playerRect.x;
            prevPlayerRect.y = playerRect.y;

            //apply gravity to yVelocity
            //assign value to point at tile past the playerRect on x-axis. If we make it to this tile without detecting collision, safe to apply gravity.
            tilePastPlayerRect = ((playerRect.x + playerWidth - 1) / tileW + 1);

            for (int i = playerRect.x / tileW; i <= tilePastPlayerRect; i++)
            {
                if (i == tilePastPlayerRect) 
                {
                    yVelocity += gravityModifier * (SDL_GetTicks64() - lastPhysicsUpdate);
                    canJump = false;
                }
                else if (TestMap::m_mapArray[(playerRect.y + playerHeight) / tileW][i] % 2 == 0)
                {
                    if (button0Down == false)
                    {
                        canJump = true;
                        yVelocity = 0.0;
                    }
                    break;
                }
            }
            
            playerRect.y += (int)(yVelocity * (SDL_GetTicks64() - lastPhysicsUpdate));
            
            
            for (int i = playerRect.x/tileW; i <= (playerRect.x + playerWidth -1) / tileW ; i++)
            {
                for (int j = playerRect.y/tileW; j <= (playerRect.y + playerHeight -1) / tileW; j++) 
                {
                    if (TestMap::m_mapArray[j][i] % 2 == 0)
                    {
                        tileCollisionRect.x = i * tileW;                        
                        tileCollisionRect.y = j * tileW;

                        SDL_IntersectRect(&playerRect, &tileCollisionRect, &collisionIntersectionRect);

                        if (prevPlayerRect.y < playerRect.y)
                        {
                            playerRect.y -= collisionIntersectionRect.h;
                            yVelocity = 0.0;
                        }
                        else if (prevPlayerRect.y > playerRect.y)
                        {
                            playerRect.y += collisionIntersectionRect.h;
                            yVelocity = 0.0;
                        }
                    }
                }
            }

            playerRect.x += (int)(xVelocity * (SDL_GetTicks64() - lastPhysicsUpdate));

            for (int i = playerRect.x / tileW; i <= (playerRect.x + playerWidth - 1) / tileW; i++)
            {
                for (int j = playerRect.y / tileW; j <= (playerRect.y + playerHeight - 1) / tileW; j++)
                {
                    if (TestMap::m_mapArray[j][i] % 2 == 0)
                    {

                        tileCollisionRect.x = i * tileW;
                        tileCollisionRect.y = j * tileW;

                        SDL_IntersectRect(&playerRect, &tileCollisionRect, &collisionIntersectionRect);


                        if (prevPlayerRect.x < playerRect.x)
                        {
                            playerRect.x -= collisionIntersectionRect.w;
                            xVelocity = 0.0;
                        }
                        else if (prevPlayerRect.x > playerRect.x)
                        {
                            playerRect.x += collisionIntersectionRect.w;
                            xVelocity = 0.0;
                        }
                    }
                }
            }
        }
        

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

            for (unsigned char i = 0; i <= (resW / tileW) + 1; i++)
            {
                for (unsigned char j = 0; j <= (resH / tileW) + 1; j++)
                {
                    textureBackgroundSky.setRectangleProperties(tileW, tileW, (i * tileW) - (cameraX % tileW), (j * tileW) - (cameraY % tileW));
                    textureBackgroundSky.render(renderer);
                }
            }

            for (int i = 0; i <= (resW / tileW) + 1; i++)
            {
                for (int j = 0; j <= (resH / tileW) + 1; j++)
                {
                    if (TestMap::m_mapArray[cameraY / tileW + j][cameraX / tileW + i] != 1)
                    {
                        if (TestMap::m_mapArray[cameraY / tileW + j][cameraX / tileW + i] == 0)
                        {
                            textureGround.setRectangleProperties(tileW, tileW, (i * tileW) - (cameraX % tileW), (j * tileW) - (cameraY % tileW));
                            textureGround.render(renderer);

                            

                        }
                        else if (TestMap::m_mapArray[cameraY / tileW + j][cameraX / tileW + i] == 6)
                        {
                            textureSpikes.setRectangleProperties(tileW, tileW, (i * tileW) - (cameraX % tileW), (j * tileW) - (cameraY % tileW));
                            textureSpikes.render(renderer);
                        }
                    }
                }
            }

            if ((playerRect.y - cameraY) > (resH * .6)) 
            {
                cameraY += ((playerRect.y - cameraY) - (resH * .6));                
                //gonna need a big fat refactor to point at current level arrays
                if (cameraY > (sizeof TestMap::m_mapArray / sizeof TestMap::m_mapArray[0]) * tileW - resH)
                {
                    cameraY = (sizeof TestMap::m_mapArray / sizeof TestMap::m_mapArray[0]) * tileW - resH;
                }
            }

            //(sizeof TestMap::m_mapArray[0] / sizeof(int))

            else if ((playerRect.y - cameraY) < (resH * .4))
            {
                cameraY -= (resH * .4) - (playerRect.y - cameraY);
                if (cameraY < 0)
                {
                    cameraY = 0;
                }
            }

            if ((playerRect.x + playerWidth / 2 - cameraX) > (resW /2))
            {
                cameraX += ((playerRect.x + playerWidth / 2 -cameraX) - (resW / 2));
                //gonna need a big fat refactor to point at current level arrays
                if (cameraX > (sizeof TestMap::m_mapArray[0] / sizeof(int)) * tileW - resW)
                {
                    cameraX = (sizeof TestMap::m_mapArray[0] / sizeof(int)) * tileW - resW;
                }
            }

            else if ((playerRect.x + playerWidth / 2 - cameraX) < (resW / 2))
            {
                cameraX -= (resW / 2) - (playerRect.x + playerWidth / 2 - cameraX);
                if (cameraX < 0)
                {
                    cameraX = 0;
                }
            }

            //prep rendering of playerRect by subtracting camera location values.
            playerDrawingRect.x = playerRect.x - cameraX;
            playerDrawingRect.y = playerRect.y - cameraY;

            SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
            SDL_RenderFillRect(renderer, &playerDrawingRect);
        }

        // Finally show what we've drawn
        SDL_RenderPresent(renderer);

        xDirLast = xDir;

        std::cout << "playerRect position: " << std::endl;
        std::cout << "x: " << playerRect.x << std::endl;
        std::cout << "y: " << playerRect.y << std::endl;
        lastPhysicsUpdate = SDL_GetTicks64();
    }


    //Close game controller
    SDL_JoystickClose(gGameController);
    gGameController = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}