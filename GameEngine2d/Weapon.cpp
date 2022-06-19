#include "Weapon.hpp"

Weapon::Weapon()
{
	name = "";
	isUnlocked = false;
	baseDamage = 0;
	numBodiesCanPenetrate = 0;
	blowbackVelocityPerShot = 0.0f;
	hasSpread = false;
	numSpreadProjectiles = 0;
	spreadMultiplier = 0.0f;
	numPixelsToBarrelEnd = 0;
	minMillisBetweenShots = 0;
	sizeOfProjectileInPixels = 0;
	isTrackedProjectile = false;
	detonates = false;
	millisUntilDetonation = 0;
	blastDamage = 0;
	blastRadius = 0;
	blastRadiusFalloffModifier = 0.0f;
	bounciness = 0.0f;
}

Weapon::~Weapon()
{
}

void Weapon::setBasicValues(std::string name, bool isUnlocked, int baseDamage, int numBodiesCanPenetrate, float blowbackVelocityPerShot, bool hasSpread,
	int numSpreadProjectiles, float spreadMultiplier, int numPixelsToBarrelEnd, int minMillisBetweenShots)
{
	this->name = name;
	this->isUnlocked = isUnlocked;
	this->baseDamage = baseDamage;
	this->numBodiesCanPenetrate = numBodiesCanPenetrate;
	this->blowbackVelocityPerShot = blowbackVelocityPerShot;
	this->hasSpread = hasSpread;
	this->numSpreadProjectiles = numSpreadProjectiles;
	this->spreadMultiplier = spreadMultiplier;
	this->numPixelsToBarrelEnd = numPixelsToBarrelEnd;
	this->minMillisBetweenShots = minMillisBetweenShots;
}

void Weapon::setProjectileValues(int sizeOfProjectileInPixels, bool isTrackedProjectile, bool detonates, int millisUntilDetonation,
	int blastDamage, int blastRadius, float blastRadiusFalloffModifier, float bounciness)
{
	this->sizeOfProjectileInPixels = sizeOfProjectileInPixels;
	this->isTrackedProjectile = isTrackedProjectile;
	this->detonates = detonates;
	this->millisUntilDetonation = millisUntilDetonation;
	this->blastDamage = blastDamage;
	this->blastRadius = blastRadius;
	this->blastRadiusFalloffModifier = blastRadiusFalloffModifier;
	this->bounciness = bounciness;
}

void Weapon::unlock()
{
	this->isUnlocked = true;
}