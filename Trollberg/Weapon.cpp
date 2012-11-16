#include "Weapon.h"


Weapon* Weapon::createWeapon(Pim::SpriteBatchNode *b, WeaponID wid)
{
	switch (wid)
	{
		case LIGHT_RIFLE:
			return new LightRifle(b);
	}

	return NULL;
}

Weapon::Weapon(Pim::SpriteBatchNode *b)
	: Pim::Sprite()
{
	actorSheet = b;
	useBatchNode(actorSheet);
}

float Weapon::damage()
{
	// Get a random number between 0 and 1
	float r = rand()/RAND_MAX;
	return minDamage + r*(maxDamage-minDamage);
}
float Weapon::angle()
{
	// Get a random number between -1 and 1
	float r = (rand()/RAND_MAX - 0.5)*2;
	return rotation + r*accuracy;
}


// ----------- LIGHT RIFLE ----------- //
LightRifle::LightRifle(Pim::SpriteBatchNode *b)
{
	accuracy  = 10.f;
	minDamage = 20;
	maxDamage = 30;
}

void LightRifle::fire()
{

}
void LightRifle::setMirrored(bool flag)
{
	scale.y = 1.f - (float)flag*2.f;
}



























