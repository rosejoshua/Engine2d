#pragma once

// C++ Standard Libraries
#include <iostream>
#include <string>

// Third-party library
#include "SDL.h"
#include "Weapon.hpp"
#include "PlayerPhysicsManager.hpp"

class WeaponsManager
{
public:
	WeaponsManager();
	~WeaponsManager();

	int selectedWeapon;

	void unlockWeapon(int weaponIndex);
	bool shoot(PlayerPhysicsManager* playerPhysicsManager, ControlsManager* controlsManager);

	Uint64 lastFiredTimestamp;
	Weapon weapon0;
	Weapon* getSelectedWeapon();
};

