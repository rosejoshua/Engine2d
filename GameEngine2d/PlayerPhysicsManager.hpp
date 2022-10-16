#pragma once

// C++ Standard Libraries
#include <iostream>

// Third-party library
#include "SDL.h"
#include "SDL_mixer.h"

#include "ControlsManager.hpp"

class PlayerPhysicsManager
{
public:
	PlayerPhysicsManager();
	~PlayerPhysicsManager();

	float jumpVelocity;
	float jumpVelocityInWater;
	float maxUpwardVelocityInWater;
	float maxDownwardVelocityInWater;
	float horVelModPlayerRunSprint;
	float horVelModPlayerWalk;
	//gravity per tick
	float gravityModifier;
	float gravityModifierInWater;
	float maxWalkVelocity;
	float maxRunVelocity;
	float maxSprintVelocity;
	float maxNegativeVelocity;
	float maxPositiveVelocity;
	float hitTheBrakes;

	float yVelocity;
	float xVelocity;
	int currentSpecialBehavior;
	bool inWater;
	bool onIce;

	Uint64 lastPhysicsUpdate;

	void updatePlayerVelocities(ControlsManager& controlsManager, Mix_Chunk* jumpSound, Mix_Chunk* thumpSound);
	void setModifiers(int playerHeight);
	void setVelocitiesZero();
};

