#pragma once

// C++ Standard Libraries
#include <iostream>
#include <string>

// Third-party library
#include "SDL.h"
#include "Weapon.hpp"

class WeaponsManager
{
public:
	WeaponsManager();
	~WeaponsManager();

	Uint64 lastFiredTimestamp;
	int selectedWeapon;
	Weapon weapon0;

	void unlockWeapon(int weaponIndex);
	Weapon* getSelectedWeapon();
};

