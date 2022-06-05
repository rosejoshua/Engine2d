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
#include "ControlsManager.hpp"

int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;
    TestMap testMap;
    ControlsManager controlsManager;
    int resW = 2500;
    int resH = 1400;
    int tileW = 60;
    int playerHeight = tileW*2.5;
    int playerWidth = tileW*1.3;

    int cameraX = 0;
    int cameraY = 0;

    const float jumpVelocity = playerHeight/-40.0;
    //const float horVelModPlayer = 0.5;
    const float horVelModPlayerRunSprint = playerHeight/1000.0;
    const float horVelModPlayerWalk = playerHeight / 4000.0;
    //gravity per tick
    const float gravityModifier = playerHeight/9000.0;
    const float maxWalkVelocity = playerHeight / 170.0;
    const float maxRunVelocity = playerHeight / 80.0;
    const float maxSprintVelocity = playerHeight / 50.0;

    Uint64 lastPhysicsUpdate = SDL_GetTicks64();
    Uint64 lastFpsCalcTime = SDL_GetTicks64();
    int framesDrawnSinceLastFpsCheck = 0;
    bool showFps = true;

    //TestMap *testMap = new TestMap();
    
    //Analog joystick dead zone
    //const int JOYSTICK_DEAD_ZONE = 5000;
    //const int WALK_SPRINT_TRANSITION = 16000;
    //SDL_Joystick* gameController = NULL;
    float yVelocity = 0.0;
    float xVelocity = 0.0;

    bool gameIsRunning = true;
    bool showMenu = true;
    int selectedMenuItem = 1;
    bool gameStarted = false;

    //joy left analog stick direction reduced to int from -3 to 3
    //int xDir = 0;
    //int xDirLast = 0;
    //int yDir = 0;

    //bool for if right analog stick aiming is taking place to draw aim line for testing
    //bool isAiming = false;

    //making possibly stupid assumption that asking for the aim direction before drawing/detecting collision is slower than storing last known direction in an int. Since using aim point CHANGE
    //events to update aiming...we don't want to stop drawing aimpoints because events stop...player could still be aiming but not moving the aim analog stick
    //Sint16 aimXDir = 0;
    //Sint16 aimYDir = 0;

    int tilePastPlayerRect = 0;

    //bool button0Down = false;
    //bool button1Down = false;
    //bool button2Down = false;
    //bool button3Down = false;
    //bool button4Down = false;
    //bool button5Down = false;
    //bool button6Down = false;
    //bool button7Down = false;
    //bool button8Down = false;
    //bool button9Down = false;
    //bool button10Down = false;
    //bool button11Down = false;
    //bool button12Down = false;

    //bool canJump = true;

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
        controlsManager.gameController = SDL_JoystickOpen(0);
        if (controlsManager.gameController == NULL)
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

    //static necessary here?
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
                controlsManager.handleJoyAxisMotion(event);
            }
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                controlsManager.handleJoyButtonDown(event);
            }
            else if (event.type == SDL_JOYBUTTONUP)
            {
                controlsManager.handleJoyButtonUp(event);
            }

        }

        // (2) Handle Updates
        // grid origin is top left of screen...
        
        //add jump to velocity
        if (controlsManager.button4Down && controlsManager.canJump) {
            yVelocity += jumpVelocity;
            controlsManager.canJump = false;
        }
        else if (!controlsManager.button4Down && !controlsManager.canJump && yVelocity < 0)
        {
            yVelocity /= 0.25*(float)(SDL_GetTicks64() - lastPhysicsUpdate);
        }

        //move player position on x axis based on xVelocity        
        if (controlsManager.xDir < 0 && controlsManager.xDirLast <= 0)
        {
            if (controlsManager.button8Down) {
                if (xVelocity > -maxSprintVelocity)
                {
                    xVelocity -= horVelModPlayerRunSprint;
                    if (xVelocity < -maxSprintVelocity)
                    {
                        xVelocity = -maxSprintVelocity;
                    }
                }
            }
            else if (controlsManager.xDir == -2)
            {
                if (xVelocity > -maxRunVelocity)
                {
                    xVelocity -= horVelModPlayerRunSprint;
                    if (xVelocity < -maxRunVelocity)
                    {
                        xVelocity = -maxRunVelocity;
                    }
                }
                else
                {
                    xVelocity = -maxRunVelocity;
                }
            }
            else
            {
                if (xVelocity > -maxWalkVelocity)
                {
                    xVelocity -= horVelModPlayerWalk;
                    if (xVelocity < -maxWalkVelocity)
                    {
                        xVelocity = -maxWalkVelocity;
                    }
                }
                else
                {
                    xVelocity = -maxWalkVelocity;
                }
            }
        }
        else if (controlsManager.xDir > 0 && controlsManager.xDirLast >= 0)
        {
            if (controlsManager.button8Down) {
                if (xVelocity < maxSprintVelocity)
                {
                    xVelocity += horVelModPlayerRunSprint;
                    if (xVelocity > maxSprintVelocity)
                    {
                        xVelocity = maxSprintVelocity;
                    }
                }
            }
            else if (controlsManager.xDir == 2)
            {
                if (xVelocity < maxRunVelocity)
                {
                    xVelocity += horVelModPlayerRunSprint;
                    if (xVelocity > maxRunVelocity)
                    {
                        xVelocity = maxRunVelocity;
                    }
                }
                else
                {
                    xVelocity = maxRunVelocity;
                }
            }
            else
            {
                if (xVelocity < maxWalkVelocity)
                {
                    xVelocity += horVelModPlayerWalk;
                    if (xVelocity > maxWalkVelocity)
                    {
                        xVelocity = maxWalkVelocity;
                    }
                }
                else
                {
                    xVelocity = maxWalkVelocity;
                }
            }
        }
        else {
            xVelocity /= 0.25 * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
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
                    controlsManager.canJump = false;
                }
                else if (testMap.m_mapArray[(playerRect.y + playerHeight) / tileW][i] % 2 == 0)
                {
                    if (controlsManager.button4Down == false)
                    {
                        controlsManager.canJump = true;
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
                    if (testMap.m_mapArray[j][i] % 2 == 0)
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
                    if (testMap.m_mapArray[j][i] % 2 == 0)
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
                    if (testMap.m_mapArray[cameraY / tileW + j][cameraX / tileW + i] != 1)
                    {
                        if (testMap.m_mapArray[cameraY / tileW + j][cameraX / tileW + i] == 0)
                        {
                            textureGround.setRectangleProperties(tileW, tileW, (i * tileW) - (cameraX % tileW), (j * tileW) - (cameraY % tileW));
                            textureGround.render(renderer);

                            

                        }
                        else if (testMap.m_mapArray[cameraY / tileW + j][cameraX / tileW + i] == 6)
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
                
                if (cameraY > (sizeof testMap.m_mapArray / sizeof testMap.m_mapArray[0]) * tileW - resH)
                {
                    cameraY = (sizeof testMap.m_mapArray / sizeof testMap.m_mapArray[0]) * tileW - resH;
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
                if (cameraX > (sizeof testMap.m_mapArray[0] / sizeof(int)) * tileW - resW)
                {
                    cameraX = (sizeof testMap.m_mapArray[0] / sizeof(int)) * tileW - resW;
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
            if (controlsManager.aimXDir > controlsManager.JOYSTICK_DEAD_ZONE || controlsManager.aimXDir < -controlsManager.JOYSTICK_DEAD_ZONE  ||
                controlsManager.aimYDir > controlsManager.JOYSTICK_DEAD_ZONE || controlsManager.aimYDir < -controlsManager.JOYSTICK_DEAD_ZONE)
            {
                SDL_RenderDrawLine(renderer, playerRect.x - cameraX + (playerWidth / 2), playerRect.y - cameraY + (playerHeight / 3), playerRect.x + (controlsManager.aimXDir), playerRect.y + (controlsManager.aimYDir));
            }
            SDL_RenderFillRect(renderer, &playerDrawingRect);
        }

        // Finally show what we've drawn
        SDL_RenderPresent(renderer);
        framesDrawnSinceLastFpsCheck++;
        if (framesDrawnSinceLastFpsCheck == 10 && showFps) {
            std::cout << "FPS: " << (10000.0 / (float)(SDL_GetTicks64() - lastFpsCalcTime)) <<  std::endl;
            lastFpsCalcTime = SDL_GetTicks64();
            framesDrawnSinceLastFpsCheck = 0;
        }

        controlsManager.xDirLast = controlsManager.xDir;

        lastPhysicsUpdate = SDL_GetTicks64();
    }


    //Close game controller
    SDL_JoystickClose(controlsManager.gameController);
    controlsManager.gameController = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}