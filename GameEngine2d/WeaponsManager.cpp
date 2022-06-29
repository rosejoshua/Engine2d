#include "WeaponsManager.hpp"

WeaponsManager::WeaponsManager()
{
	lastFiredTimestamp = SDL_GetTicks64();
	//blowbackvelocity should be pegged to jump velocity or player size
	weapon0.setBasicValues("pistol", true, 5, 1, 0.4f, false, 1, 0.0f, 10, 250);
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

bool WeaponsManager::shoot(PlayerPhysicsManager* playerPhysicsManager, ControlsManager* controlsManager)
{
	if (SDL_GetTicks64() - lastFiredTimestamp > getSelectedWeapon()->minMillisBetweenShots)
	{
		//this is faulty, just go strictly on direction, not throttle based on how far out the joystick is pressed.
		playerPhysicsManager->xVelocity += ((getSelectedWeapon()->blowbackVelocityPerShot) * (-(controlsManager->aimXDir) / 5000.0f));
		playerPhysicsManager->yVelocity += ((getSelectedWeapon()->blowbackVelocityPerShot) * (-(controlsManager->aimYDir) / 5000.0f));
			

		lastFiredTimestamp = SDL_GetTicks64();
		//if player has upward velocity, force heavy gravity to prevent combo of floaty grav and downward shooting, testing
		//if(playerPhysicsManager->yVelocity<0)
		//	playerPhysicsManager->forceHeavyGravity = true;
		return true;
	}
	else
	{
		return false;
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
