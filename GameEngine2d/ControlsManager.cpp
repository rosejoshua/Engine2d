#include "ControlsManager.hpp"

ControlsManager::ControlsManager()
{
	//joy left analog stick direction reduced to int from -3 to 3
	int xDir = 0;
	int xDirLast = 0;
	int yDir = 0;

	//bool for if right analog stick aiming is taking place to draw aim line for testing
	bool isAiming = false;

	//making possibly stupid assumption that asking for the aim direction before drawing/detecting collision is slower than storing last known direction in an int. Since using aim point CHANGE
	//events to update aiming...we don't want to stop drawing aimpoints because events stop...player could still be aiming but not moving the aim analog stick
	Sint16 aimXDir = 0;
	Sint16 aimYDir = 0;

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

    lastMenuItemChangeTime = SDL_GetTicks64();
}

ControlsManager::~ControlsManager()
{
    gameController = NULL;
    SDL_JoystickClose(gameController);
}

void ControlsManager::handleJoyAxisMotion(SDL_Event& event, bool& showMenu, int& selectedMenuItem, bool& gameStarted, bool& gameIsRunning)
{
    //Motion on controller 0
    if (event.jaxis.which == 0)
    {
        //left analog stick X axis motion
        if (event.jaxis.axis == 0)
        {
            //Left of dead zone
            if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
            {
                /*std::cout << "left stick value: " << event.jaxis.value << std::endl;*/
                if (event.jaxis.value > -WALK_SPRINT_TRANSITION)
                {
                    xDir = -1;

                }
                else
                {
                    xDir = -2;
                }
            }
            //Right of dead zone
            else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
            {
                /*std::cout << "left stick value: " << event.jaxis.value << std::endl;*/
                if (event.jaxis.value < WALK_SPRINT_TRANSITION)
                {
                    xDir = 1;
                }
                else
                {
                    xDir = 2;
                }
            }
            else
            {
                xDir = 0;
            }
        }
        //left analog stick Y axis motion
        else if (event.jaxis.axis == 1)
        {
            //Below dead zone
            if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
            {
                yDir = -1;
            }
            //Above dead zone
            else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
            {
                yDir = 1;
            }
            else
            {
                yDir = 0;
            }
        }
        //right analog stick X axis motion
        else if (event.jaxis.axis == 2)
        {
            aimXDir = event.jaxis.value;
        }
        //right analog stick Y axis motion
        else if (event.jaxis.axis == 3)
        {
            aimYDir = event.jaxis.value;
        }

    }

}

void ControlsManager::handleJoyButtonDown(SDL_Event& event, bool& showMenu, int& selectedMenuItem, bool& gameStarted, bool& gameIsRunning)
{
    if (event.jbutton.button == 0) {
        button0Down = true;
        std::cout << "button 0 pressed" << std::endl;

        if (showMenu)
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
    }
    else if (event.jbutton.button == 1) {
        button1Down = true;
        std::cout << "button 1 pressed" << std::endl;
    }
    else if (event.jbutton.button == 2) {
        button2Down = true;
        std::cout << "button 2 pressed" << std::endl;
    }
    else if (event.jbutton.button == 3) {
        button3Down = true;
        std::cout << "button 3 pressed" << std::endl;
    }
    else if (event.jbutton.button == 4) {
        button4Down = true;
        std::cout << "button 4 pressed" << std::endl;
    }
    else if (event.jbutton.button == 5) {
        button5Down = true;
        std::cout << "button 5 pressed" << std::endl;
    }
    else if (event.jbutton.button == 6) {
        button6Down = true;
        std::cout << "button 6 pressed" << std::endl;
    }
    else if (event.jbutton.button == 7) {
        button7Down = true;
        std::cout << "button 7 pressed" << std::endl;

        if (showMenu)
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
        else if (gameStarted)
        {
            showMenu = true;
            selectedMenuItem = 1;
            gameStarted = false;
        }
    }
    else if (event.jbutton.button == 8) {
        button8Down = true;
        std::cout << "button 8 pressed" << std::endl;
    }
    else if (event.jbutton.button == 9) {
        button9Down = true;
        std::cout << "button 9 pressed" << std::endl;
    }
    else if (event.jbutton.button == 10) {
        button10Down = true;
        std::cout << "button 10 pressed" << std::endl;
    }
    else if (event.jbutton.button == 11) {
        button11Down = true;
        std::cout << "button 11 pressed" << std::endl;
    }
    else if (event.jbutton.button == 12) {
        button12Down = true;
        std::cout << "button 12 pressed" << std::endl;
    }
}

void ControlsManager::handleJoyButtonUp(SDL_Event &event)
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

void ControlsManager::handleKeyDown(SDL_Event& event, bool& showMenu, int& selectedMenuItem, bool& gameStarted, bool& gameIsRunning)
{

    if (gameStarted)
    {
        //keyboard controls impl
        if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
            selectedMenuItem--;

        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            showMenu = true;
            selectedMenuItem = 1;
            gameStarted = false;
        }
    }

    else if (showMenu)
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
    }

}

void ControlsManager::initializeControls()
{
    //Check for joysticks
    if (SDL_NumJoysticks() < 1)
    {
        std::cout << "Warning: No joysticks connected!" << std::endl;
    }
    else
    {
        //Load joystick
        gameController = SDL_JoystickOpen(0);
        if (gameController == NULL)
        {
            std::cout << "Warning: Unable to open game controller! SDL Error: " << std::endl;
        }
    }
}
