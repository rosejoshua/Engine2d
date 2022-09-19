// GameEngine2d.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"

// C++ Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>

#include "ControlsManager.hpp"
#include "PlayerPhysicsManager.hpp"
#include "MenuManager.hpp"
#include "YouDiedManager.hpp"
#include "WeaponsManager.hpp"
#include "ScoreManager.hpp"
#include "TileFactory.hpp"
#include "TextureToTileMapper.hpp" 
#include "Camera.hpp"
#include "MapBuffer.hpp"
#include "LevelSeed.hpp"
#include "Bitmap.hpp"
#include "LevelMap.hpp"
#include "Sound.hpp"

// variable declarations
static Uint8* audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

int main(int argc, char* argv[]) {

    srand(time(nullptr));
    const int horizontalParallaxFactor = 3;
    const int verticalParallaxFactor = 3;
    const int numUniqueTilesInLevel = 14;
    const int cycleLimiter = 8;

    int resW = 1920;
    int resH = 1020;
    int tileW = 30;
    int playerHeight = tileW * 2.5;
    int playerWidth = tileW * 1.3;
    bool showFps = false;
    int framesDrawnSinceLastFpsCheck = 0;
    bool gameIsRunning = true;
    bool showMenu = true;
    bool gameStarted = false;
    int tilePastPlayerRect = 0;
    bool dead = false;
    Uint64 diedAt = 0;
    int xLookPos = 0;
    int yLookPos = 0;
    int hiddenBottomHeight = 4*tileW;
    float previousToGravityAppliedYVelocity = 0.0f;
    Uint64 horizontalProgress = 0;
    MyRGB menuBackgroundColor;
    MyRGB levelBackgroundColor = MyRGB(56, 192, 242);
    //MyRGB levelBackgroundColor = MyRGB(102, 153, 255);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO ) < 0) {
        std::cout << "SDL could not be initialized: " <<
            SDL_GetError();
        return 1;
    }
    else {
        std::cout << "SDL VIDEO, JOYSTICK and AUDIO system is ready to go!" << std::endl;
    }
    Uint64 lastFpsCalcTime = SDL_GetTicks64();
    SDL_Window* window = nullptr;
    ControlsManager controlsManager;
    ControlsManager* controlsManagerPtr = &controlsManager;
    MenuManager menuManager;
    YouDiedManager youDiedManager;
    ScoreManager scoreManager;
    PlayerPhysicsManager playerPhysicsManager;
    playerPhysicsManager.setModifiers(playerHeight);
    WeaponsManager weaponsManager;
    Camera camera(0 ,0, 0, 0, 30);
    
    controlsManager.initializeControls();

    window = SDL_CreateWindow("THE GAME!", 400, 200, resW, resH, /*SDL_WINDOW_FULLSCREEN |*/ SDL_WINDOW_OPENGL);

    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError();
    }

    Sound* youDiedSound;
    youDiedSound = new Sound("./sound/sad-trombone.wav");
    youDiedSound->SetupDevice();

    Sound* coinSound;
    coinSound = new Sound("./sound/coin.wav");
    coinSound->SetupDevice();

    Sound* jumpSound;
    jumpSound = new Sound("./sound/jump.wav");
    jumpSound->SetupDevice();

    Sound* splashSound;
    splashSound = new Sound("./sound/splash.wav");
    splashSound->SetupDevice();

    Sound* titleSong;
    titleSong = new Sound("./sound/title.wav");
    titleSong->SetupDevice();
    titleSong->PlaySound();


    //if (SDL_LoadWAV(audioYouDiedDir.c_str(), &sound_spec, &sound_buffer, &sound_length) == NULL) {
    //    return 1;
    //}

    //SDL_AudioDeviceID m_device;
    //m_device = SDL_OpenAudioDevice(nullptr, 0, sound_spec, nullptr, )

    //SDL_QueueAudio(m_device, sound_buffer, sound_length);
    //SDL_PauseAudioDevice(m_device,0);

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    TextureToTileMapper textureToTileMapper(renderer, tileW);
    menuManager.initialize(resW, resH, renderer);
    youDiedManager.initialize(resW, resH, renderer);
    scoreManager.initialize(resW, resH, renderer);

    // Create a rectangle for player model
    SDL_Rect playerRect;
    playerRect.w = playerWidth;
    std::cout << playerWidth << " ";
    playerRect.h = playerHeight;
    std::cout << playerHeight << std::endl;
    //playerRect.x = resW/2 - playerWidth/2 + 2*tileW;
    playerRect.x = 1000;
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
    playerDrawingRect.x = playerRect.x - camera.cameraX;
    playerDrawingRect.y = playerRect.y - camera.cameraY;
    playerDrawingRect.w = playerRect.w;
    playerDrawingRect.h = playerRect.h;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////TESTS////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MyRGB black;
    black = MyRGB(0, 0, 0);
    MyRGB white;
    white = MyRGB(255, 255, 255);

    Bitmap level1Bitmap("./levels/level1.bmp");
    Bitmap background1Bitmap("./backgrounds/background1.bmp");

    //first row of level bitmap starts with a color coding to identify what each pixel in the 
    //bitmap corresponds to as a tile
    vector<MyRGB> colorToTileId;
    //reading the tile color key in bottom left of level 1 Bitmap
    for (size_t i = 0; i < numUniqueTilesInLevel; i++)
    {
        colorToTileId.push_back(level1Bitmap.getPixel(i, 0));

        if (level1Bitmap.getPixel(i, 0) != black && level1Bitmap.getPixel(i, 0) != white)
        {
            //after reading in the tile key, the program changes it to red which is the color mapped to spikes...need lethal tile like spikes in all holes off screen so this is the safe default.
            level1Bitmap.setPixel(i, 0, 255, 0, 0);
        }
    }

    LevelMap level1(level1Bitmap.getWidth(), level1Bitmap.getHeight());
    int tileId = -1;

    //this is stupid because I refused to write my own hash function for a map<MyRGB> way to associate tileIDs with colors from the bitmap, 
    //doesn't really need to be that fast (hopefully!), only used once when each level loads. 
    for (int y = 0; y < level1Bitmap.getHeight(); y++)
    {
        for (int x = 0; x < level1Bitmap.getWidth(); x++)
        {
            for (int i = 0; i < colorToTileId.size(); i++)
            {
                if (colorToTileId[i] == level1Bitmap.getPixel(x, y))
                {
                    {
                        tileId = i;
                        break;
                    }
                }
            }
            level1.setTileIdFromBitmapArray(y, x, tileId);
        }
    }

    tileId = -1;

    LevelMap background1(background1Bitmap.getWidth(), background1Bitmap.getHeight());
    for (int y = 0; y < background1Bitmap.getHeight(); y++)
    {
        for (int x = 0; x < background1Bitmap.getWidth(); x++)
        {
            for (int i = 0; i < colorToTileId.size(); i++)
            {
                if (colorToTileId[i] == background1Bitmap.getPixel(x, y))
                {
                    {
                        tileId = i;
                        break;
                    }
                }
            }
            background1.setTileIdFromBitmapArray(y, x, tileId);
        }
    }


    //for (auto i : *level1.m_tileIds)
    //{
    //    for (auto j : i)
    //    {
    //        cout << j << " ";
    //    }
    //    cout << endl;
    //}


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////END TESTS/////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Infinite loop for our application
    // Main application loop
    while (gameIsRunning) 
    {   
        //frame capping to about 165hz...will eventually separate physics from capped drawing/display to 
        //hopefully prioritize physics calculations over drawing and not overdraw to prevent shearing on good PCs
        while (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate < cycleLimiter) {}
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
                    //for water gravity, this is to record previous velocity for knowing if we are falling. This is used to correct too much hangtime while very small 
                    //yVelocity values have no effect on player position due to casting floating points to ints (single pixel resolving of position)
                    previousToGravityAppliedYVelocity = playerPhysicsManager.yVelocity;

                    if (!playerPhysicsManager.inWater)
                    {
                        playerPhysicsManager.yVelocity += playerPhysicsManager.gravityModifier * (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate);
                        if (playerPhysicsManager.yVelocity > playerPhysicsManager.maxDownwardVerticalVelocity)
                            playerPhysicsManager.yVelocity = playerPhysicsManager.maxDownwardVerticalVelocity;
                        controlsManager.canJump = false;
                    }
                    else
                    {
                        playerPhysicsManager.yVelocity += playerPhysicsManager.gravityModifierInWater * (SDL_GetTicks64() - playerPhysicsManager.lastPhysicsUpdate);

                        //-0.16875 smallest -yVelocity causing position change
                        //-0.16375 first -yVelocity with no change
                        //0.16625 first +yVelocity with no change
                        //0.17125 smallest +yVelocity causing position change
                        // 
                        //eliminates excessive hangtime when jumping in water
                        //todo: relativize these hardcoded values to the playerHeight
                        //if (playerPhysicsManager.yVelocity < 0.0 && previousToGravityAppliedYVelocity < -0.16375 && playerPhysicsManager.yVelocity >= -0.16375)
                        if (playerPhysicsManager.yVelocity < 0.0 && previousToGravityAppliedYVelocity < -0.1 && playerPhysicsManager.yVelocity >= -0.1)
                            playerPhysicsManager.yVelocity = 0.0;
                        else if (playerPhysicsManager.yVelocity > 0.0 && previousToGravityAppliedYVelocity > 0.1 && playerPhysicsManager.yVelocity <= 0.1)
                            playerPhysicsManager.yVelocity = 0.0;
                        else if (playerPhysicsManager.yVelocity > 0.0 && playerPhysicsManager.yVelocity < 0.1 && playerPhysicsManager.yVelocity > previousToGravityAppliedYVelocity)
                            playerPhysicsManager.yVelocity = 0.1;

                        if (playerPhysicsManager.yVelocity > playerPhysicsManager.maxDownwardVelocityInWater)
                            playerPhysicsManager.yVelocity = playerPhysicsManager.maxDownwardVelocityInWater;
                    }
                }
                else if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][(playerRect.y + playerHeight) / tileW]]->isCollision)
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
                    //check if player hit its head on lethal block
                    //todo:this is going to create a bug if player hits head on lethal at the same time as non-lethal neighbor, will need some special logic 
                    //to check all tiles, or just stop gravity when player is dead and make spikes non-collision and include below
                    if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->isLethal)
                    {
                        dead = true;
                        diedAt = SDL_GetTicks64();
                        gameStarted = false;
                    }
                    //remove player from collision (immovable) blocks
                    if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->isCollision)
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
                    if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->isCollision)
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
            //todo:check standing on lethal tile or water here
            for (int i = playerRect.x / tileW; i <= (playerRect.x + playerWidth - 1) / tileW; i++)
            {
                //this purposfully looks at the pixels just below the player to see if they are standing on lethal.
                if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][(playerRect.y + playerHeight) / tileW]]->isLethal)
                {
                    dead = true;
                    diedAt = SDL_GetTicks64();
                    gameStarted = false;
                }
                    
                //checking in water, only checks bottom right tile of collision matrix
                if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][(playerRect.y + playerHeight - 1) / tileW]]->isLiquid)
                    playerPhysicsManager.inWater = true;
                else
                {
                    playerPhysicsManager.inWater = false;
                }

                //if ((*level1.m_tileIds)[i][(playerRect.y + playerHeight) / tileW] == 13)
                //{
                //    playerPhysicsManager.yVelocity += (2 * playerPhysicsManager.jumpVelocity);
                //}
            }
            //check for lethal non-collision tiles and consumables
            for (int i = playerRect.x / tileW; i <= (playerRect.x + playerWidth - 1) / tileW; i++)
            {
                for (int j = playerRect.y / tileW; j <= (playerRect.y + playerHeight - 1) / tileW; j++)
                {
                    if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->isConsumable)
                    {
                        //check if tile needs more collision than just one pixel, used for items that are smaller than the tile size
                        //or for cutting the player a little slack with lethal items
                        if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->minimumCollisionRatio != 0)
                        {
                            tileCollisionRect.x = i * tileW;
                            tileCollisionRect.y = j * tileW;
                            SDL_IntersectRect(&playerRect, &tileCollisionRect, &collisionIntersectionRect);
                            if ((collisionIntersectionRect.w * collisionIntersectionRect.h) >
                                ((tileW * tileW) / textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->minimumCollisionRatio))
                            {
                                scoreManager.addConsumablePoints((*level1.m_tileIds)[i][j]);
                                //todo:call transform() here when complete
                                (*level1.m_tileIds)[i][j] = textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->transformationTileIndex;
                            }
                        }
                        else
                        {
                            scoreManager.addConsumablePoints((*level1.m_tileIds)[i][j]);
                            //todo:call transform() here when complete
                            (*level1.m_tileIds)[i][j] = textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->transformationTileIndex;
                        }
                    }
                    else if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->isLethal)
                    {
                        dead = true;
                        diedAt = SDL_GetTicks64();
                        gameStarted = false;
                    }
                        
                }
            }
        }
        
        if (dead)
        {
            showMenu = false;
            youDiedManager.drawYouDied(renderer, &dead, diedAt, &gameStarted, &showMenu, resW, resH, youDiedSound);
            if (!dead)
            {
                playerRect.x = 1000;
                playerRect.y = tileW;
                std::cout << "are we here???" << std::endl;
                std::cout << "showMenu: " << showMenu << std::endl;
            }
        }
        if (showMenu)
        {
            // (3) Clear and Draw the Screen
            // Gives us a clear "canvas"
            SDL_SetRenderDrawColor(renderer, menuBackgroundColor.r, menuBackgroundColor.g, menuBackgroundColor.b, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            menuManager.drawMenu(controlsManagerPtr, renderer);
        }
            

        if (gameStarted && !dead) 
        {
            titleSong->StopSound();
            // (3) Clear and Draw the Screen
            // Gives us a clear "canvas"
            SDL_SetRenderDrawColor(renderer, levelBackgroundColor.r, levelBackgroundColor.g, levelBackgroundColor.b, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            //moved camera calculations above tile drawing to eliminate bug showing leftmost tiles in array before drawing based on player positon. Hopefully this doesn't cause any issues (so far so good)
            if ((playerRect.y - camera.cameraY) > (resH * .6))
            {
                camera.cameraY += ((playerRect.y - camera.cameraY) - (resH * .6));
                //hiding bottom set of tiles so that player can fall thru holes in the ground
                if (camera.cameraY > (level1.m_height * tileW - resH) - hiddenBottomHeight)
                    camera.cameraY = (level1.m_height * tileW - resH) - hiddenBottomHeight;
            }
            else if ((playerRect.y - camera.cameraY) < (resH * .4))
            {
                camera.cameraY -= (resH * .4) - (playerRect.y - camera.cameraY);
                if (camera.cameraY < 0)
                    camera.cameraY = 0;
            }

            if ((playerRect.x + playerWidth / 2 - camera.cameraX) > (resW / 2))
            {
                camera.cameraX += ((playerRect.x + playerWidth / 2 - camera.cameraX) - (resW / 2));
                //if ((playerRect.x + playerWidth / 2) > (sizeof testMap.m_mapArray[0] / sizeof(int) + 2*tileW + resW / 2))
                //{
                //    playerRect.x -= tileW;
                //    camera.cameraX -= tileW;

                //    horizontalProgress++;
                //    scoreManager.updateScoreByProgress(horizontalProgress);

                //    call regen on map array
                //    for (int i = 2; i < testMap.WIDTH - 1; i++)
                //    {
                //        for (int j = 0; j < sizeof testMap.m_mapArray / sizeof testMap.m_mapArray[0]; j++)
                //        for (int j = 0; j < testMap.HEIGHT; j++)
                //        {
                //            testMap.m_mapArray[j][i - 1] = testMap.m_mapArray[j][i];

                //        }
                //    }

                //    for (int i = 1; i < testMap.HEIGHT-3; i++)
                //    {                        
                //        tileFactory.copyTile(testMap.m_mapArray[i][testMap.WIDTH - 2], i);
                //    }
                //    tileFactory.cycleTileQueue();
                //}
            }
            else if ((playerRect.x + playerWidth / 2 - camera.cameraX) < (resW / 2))
            {
                camera.cameraX -= (resW / 2) - (playerRect.x + playerWidth / 2 - camera.cameraX);
            }

            //camera bounds checking and corrections
            if (camera.cameraX < tileW)
                camera.cameraX = tileW;

            if (camera.cameraX > level1.m_width * tileW - resW - tileW)
                camera.cameraX = level1.m_width * tileW - resW - tileW;
            //Draw background tiles first
            for (int i = 0; i < (resW / tileW) + 1; i++)
            {
                //[(camera.cameraX / 3) / tileW + (i / 3)][(camera.cameraY / 3) / tileW + (j / 3)]
                for (int j = 0; j <= (resH / tileW); j++)
                {
                    textureToTileMapper.drawTile(renderer, (*background1.m_tileIds)[
                        
                        (((camera.cameraX / horizontalParallaxFactor) / tileW) + i) % background1.m_width
                    
                    ][
                        
                        (((camera.cameraY / verticalParallaxFactor) / tileW) + j) % background1.m_height
                    
                    ],
                        (i * tileW) - (camera.cameraX/horizontalParallaxFactor % tileW),
                        (j * tileW) - (camera.cameraY/verticalParallaxFactor % tileW),
                        SDL_GetTicks64());
                }
            }

            for (int i = 0; i < (resW / tileW) + 1; i++)
            {
                //<= necessary for drawing tiles to fill space less than tileW (underbuffering). Added hidden row of tiles to bottom 
                //to prevent out of range errors accessing bottom row of tiles
                for (int j = 0; j <= (resH / tileW) ; j++)
                {
                    if ((*level1.m_tileIds)[camera.cameraX / tileW + i][camera.cameraY / tileW + j] != 1)
                    {
                        textureToTileMapper.drawTile(renderer, (*level1.m_tileIds)[camera.cameraX / tileW + i][camera.cameraY / tileW + j],
                            (i * tileW) - (camera.cameraX % tileW),
                            (j * tileW) - (camera.cameraY % tileW),
                            SDL_GetTicks64());
                    }
                }
            }

            //prep rendering of playerRect by subtracting camera location values.
            playerDrawingRect.x = playerRect.x - camera.cameraX;
            playerDrawingRect.y = playerRect.y - camera.cameraY;

            scoreManager.drawScore(renderer);

            if (!dead)
                SDL_SetRenderDrawColor(renderer, 255, 105, 180, SDL_ALPHA_OPAQUE);
            else SDL_SetRenderDrawColor(renderer, 125, 125, 125, SDL_ALPHA_OPAQUE);

            //if (controlsManager.aimXDir > controlsManager.JOYSTICK_DEAD_ZONE || controlsManager.aimXDir < -controlsManager.JOYSTICK_DEAD_ZONE  ||
            //    controlsManager.aimYDir > controlsManager.JOYSTICK_DEAD_ZONE || controlsManager.aimYDir < -controlsManager.JOYSTICK_DEAD_ZONE)
            //{
            //    SDL_RenderDrawLine(renderer, playerRect.x - cameraX + (playerWidth / 2), playerRect.y - cameraY + (playerHeight / 3), playerRect.x + (controlsManager.aimXDir), playerRect.y + (controlsManager.aimYDir));
            //}

            //Right aimstick line drawing for testing, no use for aim yet.
            //if (controlsManager.aimXDir > controlsManager.JOYSTICK_DEAD_ZONE || controlsManager.aimXDir < -controlsManager.JOYSTICK_DEAD_ZONE ||
            //    controlsManager.aimYDir > controlsManager.JOYSTICK_DEAD_ZONE || controlsManager.aimYDir < -controlsManager.JOYSTICK_DEAD_ZONE)
            //{
            //    controlsManager.getNormalizedRightStickDir(&xLookPos, &yLookPos, camera.lookModifierValue);
            //    SDL_RenderDrawLine(renderer, playerRect.x - camera.cameraX + (playerWidth / 2), playerRect.y - camera.cameraY + (playerHeight / 3), 
            //        playerRect.x - camera.cameraX + (playerWidth / 2) + xLookPos, playerRect.y - camera.cameraY + (playerHeight / 3) + yLookPos);
            //}
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
    delete youDiedSound;
    delete splashSound;
    delete jumpSound;
    delete coinSound;
    //delete titleSong;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}