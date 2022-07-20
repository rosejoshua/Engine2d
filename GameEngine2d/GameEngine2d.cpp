// GameEngine2d.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>

#include "TextureRectangle.hpp"
#include "TestMap.hpp"
#include "ControlsManager.hpp"
#include "PlayerPhysicsManager.hpp"
#include "MenuManager.hpp"
#include "WeaponsManager.hpp"
#include "ScoreManager.hpp"
#include "TileFactory.hpp"

int main(int argc, char* argv[]) {

    srand(time(nullptr));
    
    int resW = 2560;
    int resH = 1440;
    int tileW = 60;
    int playerHeight = tileW * 2.5;
    int playerWidth = tileW * 1.3;
    int cameraX = 0;
    int cameraY = 0;
    bool showFps = true;
    int framesDrawnSinceLastFpsCheck = 0;
    bool gameIsRunning = true;
    bool showMenu = true;
    bool gameStarted = false;
    int tilePastPlayerRect = 0;
    bool dead = false;
    Uint64 horizontalProgress = 0;


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "SDL could not be initialized: " <<
            SDL_GetError();
        return 1;
    }
    else {
        std::cout << "SDL video system is ready to go!" << std::endl;
    }


    Uint64 lastFpsCalcTime = SDL_GetTicks64();
    SDL_Window* window = nullptr;
    TestMap testMap;
    ControlsManager controlsManager;
    ControlsManager* controlsManagerPtr = &controlsManager;
    MenuManager menuManager;
    ScoreManager scoreManager;
    PlayerPhysicsManager playerPhysicsManager;
    playerPhysicsManager.setModifiers(playerHeight);
    WeaponsManager weaponsManager;
    TileFactory tileFactory(testMap.HEIGHT, 400, tileW, playerHeight, playerWidth);

    controlsManager.initializeControls();

    window = SDL_CreateWindow("THE GAME!", 0, 0, resW, resH,
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError();
    }

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    menuManager.initialize(resW, resH, renderer);
    scoreManager.initialize(resW, resH, renderer);
    
    //************GET RID OF TextureRectangle CLASS ASAP**************
    TextureRectangle textureBackgroundSky = TextureRectangle(renderer, "./images/TestBackgroundTile.bmp");
    TextureRectangle textureGround = TextureRectangle(renderer, "./images/ground.bmp");
    TextureRectangle textureSpikes = TextureRectangle(renderer, "./images/spikes.bmp");

    // Create a rectangle for player model
    SDL_Rect playerRect;
    playerRect.w = playerWidth;
    playerRect.h = playerHeight;
    //playerRect.x = resW/2 - playerWidth/2 + 2*tileW;
    playerRect.x = sizeof(testMap.m_mapArray[0]) / sizeof(int) * tileW / 2 - playerRect.w/2;
    playerRect.y = tileW;

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
        //frame capping to about 165hz...will eventually separate physics from capped drawing/display to 
        //hopefully prioritize physics calculations over drawing and not overdraw to prevent shearing on good PCs
        while (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate < 6) {}
        SDL_Event event;

        // (1) Handle Input
        // Start our event loop
        while (SDL_PollEvent(&event)) 
        {
            // Handle each specific event
            if (event.type == SDL_QUIT) 
            {
                gameIsRunning = false;
            }            
            else if (event.type == SDL_JOYAXISMOTION)
            {
                controlsManager.handleJoyAxisMotion(event, showMenu, menuManager.selectedMenuItem, gameStarted, gameIsRunning);
            }
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                controlsManager.handleJoyButtonDown(event, showMenu, menuManager.selectedMenuItem, gameStarted, gameIsRunning);
            }
            else if (event.type == SDL_JOYBUTTONUP)
            {
                controlsManager.handleJoyButtonUp(event);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                controlsManager.handleKeyDown(event, showMenu, menuManager.selectedMenuItem, gameStarted, gameIsRunning);
            }

        }

        // (2) Handle Updates
        // grid origin is top left of screen...
        playerPhysicsManager.updatePlayerVelocities(controlsManager);

        if (gameStarted) 
        {
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
                    playerPhysicsManager.yVelocity += playerPhysicsManager.gravityModifier * (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate);
                    controlsManager.canJump = false;
                }
                //else if (testMap.m_mapArray[(playerRect.y + playerHeight) / tileW][i] % 6 == 0)
                //{
                //    dead = true;
                //}
                else if (testMap.m_mapArray[(playerRect.y + playerHeight) / tileW][i] % 2 == 0)
                {
                    if (controlsManager.button0Down == false)
                    {
                        controlsManager.canJump = true;
                        playerPhysicsManager.yVelocity = 0.0;
                    }
                    break;
                }
            }
            
            playerRect.y += (int)(playerPhysicsManager.yVelocity * (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate));
            
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
                            playerPhysicsManager.yVelocity = 0.0;
                        }
                        else if (prevPlayerRect.y > playerRect.y)
                        {
                            playerRect.y += collisionIntersectionRect.h;
                            playerPhysicsManager.yVelocity = 0.0;
                        }
                    }
                }
            }
            playerRect.x += (int)(playerPhysicsManager.xVelocity * (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate));

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
                            playerPhysicsManager.xVelocity = 0.0;
                        }
                        else if (prevPlayerRect.x > playerRect.x)
                        {
                            playerRect.x += collisionIntersectionRect.w;
                            playerPhysicsManager.xVelocity = 0.0;
                        }
                    }
                }
            }
        }
        

        // (3) Clear and Draw the Screen
        // Gives us a clear "canvas"
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (showMenu) 
            menuManager.drawMenu(controlsManagerPtr, renderer);

        if (gameStarted) 
        {
            //moved camera calculations above tile drawing to eliminate bug showing leftmost tiles in array before drawing based on player positon. Hopefully this doesn't cause any issues (so far so good)
            if ((playerRect.y - cameraY) > (resH * .6))
            {
                cameraY += ((playerRect.y - cameraY) - (resH * .6));
                if (cameraY > (sizeof testMap.m_mapArray / sizeof testMap.m_mapArray[0]) * tileW - resH)
                    cameraY = (sizeof testMap.m_mapArray / sizeof testMap.m_mapArray[0]) * tileW - resH;
            }
            else if ((playerRect.y - cameraY) < (resH * .4))
            {
                cameraY -= (resH * .4) - (playerRect.y - cameraY);
                if (cameraY < 0)
                    cameraY = 0;
            }

            if ((playerRect.x + playerWidth / 2 - cameraX) > (resW / 2))
            {
                cameraX += ((playerRect.x + playerWidth / 2 - cameraX) - (resW / 2));
                if (cameraX > (sizeof testMap.m_mapArray[0] / sizeof(int)) * tileW - resW - tileW)
                    cameraX = (sizeof testMap.m_mapArray[0] / sizeof(int)) * tileW - resW - tileW;
                if ((playerRect.x + playerWidth / 2) > (sizeof testMap.m_mapArray[0] / sizeof(int) + 2*tileW + resW / 2))
                {
                    playerRect.x -= tileW;
                    cameraX -= tileW;

                    horizontalProgress++;
                    scoreManager.updateScoreByProgress(horizontalProgress);

                    //call regen on map array
                    for (int i = 2; i < testMap.WIDTH - 1; i++)
                    {
                        //for (int j = 0; j < sizeof testMap.m_mapArray / sizeof testMap.m_mapArray[0]; j++)
                        for (int j = 0; j < testMap.HEIGHT; j++)
                        {
                            testMap.m_mapArray[j][i - 1] = testMap.m_mapArray[j][i];

                        }
                    }
                    testMap.m_mapArray[testMap.HEIGHT - 2][testMap.WIDTH - 2] = 6;
                    for (int i = 1; i < testMap.HEIGHT-3; i++)
                    {                        
/*                        if (rand() % 10 > 8)
                        {
                            testMap.m_mapArray[i][testMap.WIDTH - 2] = rand() % 2;
                        }
                        else
                        {
                            testMap.m_mapArray[i][testMap.WIDTH - 2] = 1;
                        }  */
                        tileFactory.copyTile(testMap.m_mapArray[i][testMap.WIDTH - 2], i);
                    }
                    tileFactory.cycleTileQueue();
                }
            }
            else if ((playerRect.x + playerWidth / 2 - cameraX) < (resW / 2))
            {
                cameraX -= (resW / 2) - (playerRect.x + playerWidth / 2 - cameraX);
                if (cameraX < tileW)
                    cameraX = tileW;
            }
            
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


            //prep rendering of playerRect by subtracting camera location values.
            playerDrawingRect.x = playerRect.x - cameraX;
            playerDrawingRect.y = playerRect.y - cameraY;

            scoreManager.drawScore(renderer);

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
        playerPhysicsManager.lastPhysicsUpdate = SDL_GetTicks64();
    }

    //Destroy shit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}