// GameEngine2d.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Third-party library
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

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
#include "PlayerSpriteManager.hpp"
#include "Background.hpp"
#include "BackgroundManager.hpp"

// variable declarations
static Uint8* audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

int main(int argc, char* argv[]) {

    srand(time(nullptr));
    const int horizontalParallaxFactor = 12;
    const int verticalParallaxFactor = 15;
    const int numUniqueTilesInLevel = 17;
    const int cycleLimiter = 6;

    int resW = 2560;
    int resH = 1440;
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "Error: " << Mix_GetError() << std::endl;
    }

    Uint64 lastFpsCalcTime = SDL_GetTicks64();
    Uint64 cameraAdjustTimestamp = SDL_GetTicks64();
    double cameraElasticity = 0.5;
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

    window = SDL_CreateWindow("THE GAME!", 100, 100, resW, resH, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError();
    }

    //Sound* youDiedSound;
    //youDiedSound = new Sound("./sound/sad-trombone.wav");
    //youDiedSound->SetupDevice();

    Mix_Chunk* coinSound = Mix_LoadWAV("./sound/coin.wav");
    Mix_Chunk* jumpSound = Mix_LoadWAV("./sound/jump.wav");
    Mix_Chunk* splashSound = Mix_LoadWAV("./sound/splash.wav");
    Mix_Chunk* thumpSound = Mix_LoadWAV("./sound/thump.wav");
    Mix_Chunk* splatSound = Mix_LoadWAV("./sound/splat.wav");

    Mix_Volume(1, 50);
    Mix_Volume(2, 40);
    Mix_Volume(3, 110);
    Mix_Volume(4, 60);

    //Sound* jumpSound;
    //jumpSound = new Sound("./sound/jump.wav");
    //jumpSound->SetupDevice();

    //Sound* splashSound;
    //splashSound = new Sound("./sound/splash.wav");
    //splashSound->SetupDevice();

    //Sound* titleSong;
    //titleSong = new Sound("./sound/title.wav");
    //titleSong->SetupDevice();
    //titleSong->PlaySound();


    //if (SDL_LoadWAV(audioYouDiedDir.c_str(), &sound_spec, &sound_buffer, &sound_length) == NULL) {
    //    return 1;
    //}

    //SDL_AudioDeviceID m_device;
    //m_device = SDL_OpenAudioDevice(nullptr, 0, sound_spec, nullptr, )

    //SDL_QueueAudio(m_device, sound_buffer, sound_length);
    //SDL_PauseAudioDevice(m_device,0);

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TextureToTileMapper textureToTileMapper(renderer, tileW);
    menuManager.initialize(resW, resH, renderer);
    youDiedManager.initialize(resW, resH, renderer);
    scoreManager.initialize(resW, resH, renderer);

    // Create a rectangle for player model
    SDL_Rect playerRect;
    playerRect.w = playerWidth;
    playerRect.h = playerHeight;
    //playerRect.x = 1000;
    //playerRect.y = tileW;

    Bitmap level1Bitmap("./levels/level1.bmp");
    //Bitmap background1Bitmap("./backgrounds/background1.bmp");
    //Background bottomNoRepeat(renderer, "./backgrounds/mountain_background.bmp", 0, 0, true, false);
    //Background cloudsRepeat(renderer, "./backgrounds/sky_background.bmp", 1, 1, true, true);

    //int startHeight = level1Bitmap.getHeight() * 30 - resW;
    int startHeight = level1Bitmap.getHeight() * tileW - 1000;
    int startWidth = playerRect.x = resW / 2 - playerWidth / 2 + 2 * tileW;
    playerRect.y = startHeight;
    playerRect.x = startWidth;

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
    SDL_Rect* p_playerDrawingRect = &playerDrawingRect;
    playerDrawingRect.x = playerRect.x - camera.cameraX;
    playerDrawingRect.y = playerRect.y - camera.cameraY;
    playerDrawingRect.w = playerRect.w;
    playerDrawingRect.h = playerRect.h;

    MyRGB black;
    black = MyRGB(0, 0, 0);
    MyRGB white;
    white = MyRGB(255, 255, 255);

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

    BackgroundManager backgroundManager(renderer, "./backgrounds/", horizontalParallaxFactor, verticalParallaxFactor, level1.m_height, tileW);

    //playerRect.x = level1.m_width * 30 - 1000;

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

    PlayerSpriteManager playerSpriteManager(renderer);

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
        playerPhysicsManager.updatePlayerVelocities(controlsManager, jumpSound, thumpSound);
        //std::cout << "after physics Update Velocities: " << playerPhysicsManager.currentSpecialBehavior << std::endl;


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

                    //std::cout << "in elseif for isCollision: " << playerPhysicsManager.currentSpecialBehavior << std::endl;

                    if (controlsManager.button0Down == false  && playerPhysicsManager.currentSpecialBehavior == 0)
                    {
                        controlsManager.canJump = true;
                    }
                    else if ((playerPhysicsManager.currentSpecialBehavior == 2) && playerPhysicsManager.yVelocity == 0.0)
                    {
                        playerPhysicsManager.currentSpecialBehavior = 0;
                    }
                    else if ((playerPhysicsManager.currentSpecialBehavior == 5) && playerPhysicsManager.yVelocity == 0.0)
                    {
                        playerPhysicsManager.currentSpecialBehavior = 0;
                    }
                    break;
                }
                //std::cout << "after elseif for isCollision: " << playerPhysicsManager.currentSpecialBehavior << std::endl;
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
                        Mix_PlayChannel(4, splatSound, 0);
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
                {
                    if (!playerPhysicsManager.inWater)
                        Mix_PlayChannel(4, splashSound, 0);
                    playerPhysicsManager.inWater = true;
                    
                }
                else
                {
                    playerPhysicsManager.inWater = false;
                }
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
                                //todo:ANOTHER FUCKING MANAGER CLASS HERE FOR SOUND
                                Mix_PlayChannel(1, coinSound, 0);
                                //todo:call transform() here when complete
                                (*level1.m_tileIds)[i][j] = textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->transformationTileIndex;
                            }
                        }
                        else
                        {
                            scoreManager.addConsumablePoints((*level1.m_tileIds)[i][j]);
                            //todo:ANOTHER FUCKING MANAGER CLASS HERE FOR SOUND
                            Mix_PlayChannel(1, coinSound, 0);
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
                    
                    else if (textureToTileMapper.intToTextureTileVector[(*level1.m_tileIds)[i][j]]->hasSpecialCharacteristics)
                    {
                        //std::cout << "in elseif for isSpecialChara: " << playerPhysicsManager.currentSpecialBehavior << std::endl;
                        if ((*level1.m_tileIds)[i][j] == 13)
                        {
                            playerPhysicsManager.currentSpecialBehavior = 1;
                        }
                        else if ((*level1.m_tileIds)[i][j] == 14)
                        {
                            playerPhysicsManager.currentSpecialBehavior = 3;
                        }
                        else if ((*level1.m_tileIds)[i][j] == 15)
                        {
                            playerPhysicsManager.currentSpecialBehavior = 4;
                        }
                        else if ((*level1.m_tileIds)[i][j] == 16)
                        {
                            playerPhysicsManager.currentSpecialBehavior = 6;
                        }
                    }
                        
                }
            }
        }
        
        if (dead)
        {
            showMenu = false;
            youDiedManager.drawYouDied(renderer, &dead, diedAt, &gameStarted, &showMenu, resW, resH/*, youDiedSound*/);
            if (!dead)
            {
                playerRect.x = startWidth;
                playerRect.y = startHeight;
            }
        }
        else if (showMenu)
        {
            // (3) Clear and Draw the Screen
            // Gives us a clear "canvas"
            SDL_SetRenderDrawColor(renderer, menuBackgroundColor.r, menuBackgroundColor.g, menuBackgroundColor.b, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            menuManager.drawMenu(controlsManagerPtr, renderer);
        }
            

        else if (gameStarted && !dead) 
        {
            //titleSong->StopSound();
            // (3) Clear and Draw the Screen
            // Gives us a clear "canvas"
            SDL_SetRenderDrawColor(renderer, levelBackgroundColor.r, levelBackgroundColor.g, levelBackgroundColor.b, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            
            //moved camera calculations above tile drawing to eliminate bug showing leftmost tiles in array before drawing based on player positon. Hopefully this doesn't cause any issues (so far so good)
            if ((playerRect.y - camera.cameraY) > (resH * .6))
            {
                camera.cameraY += (cameraElasticity * ((playerRect.y - camera.cameraY) - (resH * .6)) + 1);
                //hiding bottom set of tiles so that player can fall thru holes in the ground
                if (camera.cameraY > (level1.m_height * tileW - resH) - hiddenBottomHeight)
                    camera.cameraY = (level1.m_height * tileW - resH) - hiddenBottomHeight;
            }
            else if ((playerRect.y - camera.cameraY) < (resH * .4))
            {
                camera.cameraY -= (cameraElasticity * ((resH * .4) - (playerRect.y - camera.cameraY)) + 1);
                if (camera.cameraY < 0)
                    camera.cameraY = 0;
            }

            if ((playerRect.x + playerWidth / 2 - camera.cameraX) > (resW / 2))
            {
                camera.cameraX += (cameraElasticity * ((playerRect.x + playerWidth / 2 - camera.cameraX) - (resW / 2)) + 1);
            }
            else if ((playerRect.x + playerWidth / 2 - camera.cameraX) < (resW / 2))
            {
                camera.cameraX -= (cameraElasticity * ((resW / 2) - (playerRect.x + playerWidth / 2 - camera.cameraX)) + 1);
            }

            //camera bounds checking and corrections
            if (camera.cameraX < tileW)
                camera.cameraX = tileW;

            if (camera.cameraX > level1.m_width * tileW - resW - tileW)
                camera.cameraX = level1.m_width * tileW - resW - tileW;

            cameraAdjustTimestamp = SDL_GetTicks64();

            //Draw background layers
            backgroundManager.drawBackground(camera.cameraX, camera.cameraY, resW, resH);
            
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
            playerSpriteManager.drawSprite(renderer, 
            p_playerDrawingRect,
            controlsManagerPtr,
            (std::abs(playerPhysicsManager.yVelocity) < 1e-4 ? false : true),
            (std::abs(playerPhysicsManager.xVelocity) < .2) ? 0 : playerPhysicsManager.xVelocity > .2 ? 1 : -1
            );
        }

        // Finally show what we've drawn
        SDL_RenderPresent(renderer);
        framesDrawnSinceLastFpsCheck++;
        if (showFps && framesDrawnSinceLastFpsCheck == 10) {
            std::cout << "FPS: " << (10000.0 / (float)(SDL_GetTicks64() - lastFpsCalcTime)) <<  std::endl;
            lastFpsCalcTime = SDL_GetTicks64();
            framesDrawnSinceLastFpsCheck = 0;
        }

        controlsManager.xDirLast = controlsManager.xDir;
        playerPhysicsManager.lastPhysicsUpdate = SDL_GetTicks64();
    }

    //Destroy shit
    //delete youDiedSound;
    //delete splashSound;
    //delete jumpSound;
    //delete coinSound;
    //delete titleSong;

    Mix_FreeChunk(coinSound);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(splashSound);
    Mix_FreeChunk(thumpSound);
    Mix_FreeChunk(splatSound);
    coinSound = nullptr;
    jumpSound = nullptr;
    splashSound = nullptr;
    thumpSound = nullptr;
    splatSound = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    SDL_Quit();
    return 0;
}