#pragma once

// C++ Standard Libraries
#include <iostream>

// Third-party library
#include "SDL.h"

class ControlsManager
{
public:
	ControlsManager();
	~ControlsManager();

	SDL_Joystick* gameController;
	const int JOYSTICK_DEAD_ZONE = 5000;
	const int WALK_SPRINT_TRANSITION = 16000;
	int xDir;
	int xDirLast;
	int yDir;
	bool isAiming;
	Sint16 aimXDir;
	Sint16 aimYDir;
	bool button0Down;
	bool button1Down;
	bool button2Down;
	bool button3Down;
	bool button4Down;
	bool button5Down;
	bool button6Down;
	bool button7Down;
	bool button8Down;
	bool button9Down;
	bool button10Down;
	bool button11Down;
	bool button12Down;
	bool canJump;

	void handleJoyAxisMotion(SDL_Event &event);
	void handleJoyButtonDown(SDL_Event &event);
	void handleJoyButtonUp(SDL_Event &event);
};

