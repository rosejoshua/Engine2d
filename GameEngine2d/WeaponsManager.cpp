#include "WeaponsManager.hpp"

WeaponsManager::WeaponsManager()
{
	lastFiredTimestamp = SDL_GetTicks64();
	weapon0.setBasicValues("pistol", true, 5, 1, 3.0f, false, 1, 0.0f, 10, 0.5f);
	selectedWeapon = 0;
}

WeaponsManager::~WeaponsManager()
{
}

void WeaponsManager::unlockWeapon(int weaponIndex)
{
	switch (weaponIndex)
	{
	case 0:
		weapon0.unlock();
		break;
	}
}

Weapon* WeaponsManager::getSelectedWeapon()
{
	switch(selectedWeapon)
	{
		case 0:
			return &weapon0;
			break;

		default:
			return nullptr;
}
	
}
