#pragma once

// C++ Standard Libraries
#include <iostream>

// Third-party library
#include "SDL.h"

#include "ControlsManager.hpp"

class PlayerPhysicsManager
{
public:
	PlayerPhysicsManager();
	~PlayerPhysicsManager();

	float jumpVelocity;

	float horVelModPlayerRunSprint;
	float horVelModPlayerWalk;

	//gravity per tick
	float gravityModifier;
	float maxWalkVelocity;
	float maxRunVelocity;
	float maxSprintVelocity;

	float yVelocity;
	float xVelocity;

	Uint64 lastPhysicsUpdate;

	void updatePlayerVelocities(ControlsManager& controlsManager);
	void setModifiers(int playerHeight);
};

