// GameEngine2d.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// On linux compile with:
// g++ -std=c++17 main.cpp glad/src/glad.c -I./glad/include -o prog -lSDL2 -ldl
// On windows compile with (if using mingw)
// g++ main.cpp ./glad/src/glad.c -I./glad/include -o prog.exe -lmingw32 -lSDL2main -lSDL2

// Third-party library
#include "SDL.h"

// C++ Standard Libraries
#include <iostream>


int main(int argc, char* argv[]) {
    // Create a window data type
    // This pointer will point to the 
    // window that is allocated from SDL_CreateWindow
    SDL_Window* window = nullptr;

    // Initialize the video subsystem.
    // iF it returns less than 1, then an
    // error code will be received.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized: " <<
            SDL_GetError();
    }
    else {
        std::cout << "SDL video system is ready to go\n";
    }
    // Request a window to be created for our platform
    // The parameters are for the title, x and y position,
    // and the width and height of the window.
    window = SDL_CreateWindow("C++ SDL2 Window", 20, 20, 640, 480, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* surface = SDL_LoadBMP("./images/test.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Create a rectangle
    SDL_Rect rectangle;
    rectangle.x = 50;
    rectangle.y = 100;
    rectangle.w = 200;
    rectangle.h = 200;


    // Infinite loop for our application
    bool gameIsRunning = true;
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

        // Do our drawing
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 5, 5, 100, 120);

        //        SDL_RenderDrawRect(renderer,&rectangle);
        SDL_RenderCopy(renderer, texture, NULL, &rectangle);


        // Finally show what we've drawn
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(texture);
    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the 
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(window);

    // our program.
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
