#pragma once

#include <string>

class Weapon
{
public:
	Weapon();
	~Weapon();

	std::string name;
	bool isUnlocked;
	int baseDamage;
	int numBodiesCanPenetrate;
	float blowbackVelocityPerShot;
	bool hasSpread;
	int numSpreadProjectiles;
	float spreadMultiplier;
	int numPixelsToBarrelEnd;
	int minMillisBetweenShots;
	
	
	//for tracked projectiles such as grenades, cannonballs, etc.
	int sizeOfProjectileInPixels;
	bool isTrackedProjectile;
	bool detonates;
	int millisUntilDetonation;
	int blastDamage;
	int blastRadius;
	float blastRadiusFalloffModifier;
	float bounciness;

	void setBasicValues(std::string name, bool isUnlocked, int baseDamage, int numBodiesCanPenetrate, float blowbackVelocityPerShot, bool hasSpread,
		int numSpreadProjectiles, float spreadMultiplier, int numPixelsToBarrelEnd, int minMillisBetweenShots);

	void setProjectileValues(int sizeOfProjectileInPixels, bool isTrackedProjectile, bool detonates, int millisUntilDetonation,
		int blastDamage, int blastRadius, float blastRadiusFalloffModifier, float bounciness);

	void unlock();

};

