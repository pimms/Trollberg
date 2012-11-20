#include "Weapon.h"
#include "GameScene.h"

#include "LightRifle.h"

Weapon* Weapon::createWeapon(Pim::SpriteBatchNode *b, WeaponID wid)
{
	switch (wid)
	{
		case LIGHT_RIFLE:
			return new LightRifle(b);
	}

	return NULL;
}

Weapon::Weapon(Pim::SpriteBatchNode *b, Pim::Rect r)
	: Pim::Sprite()
{
	listenFrame();

	actorSheet	= b;
	rect		= r;	
	timer		= 10.f;			// Initial high value, instant firing is allowed
	timerAlt	= 10.f;	
	useBatchNode(actorSheet);
}

float Weapon::damage()
{
	// Get a random number between 0 and 1
	float r = (float)rand()/(float)RAND_MAX;
	return minDamage + r*(maxDamage-minDamage);
}
float Weapon::angle()
{
	// Get a random number between -1 and 1
	float r = (float)rand()/((float)RAND_MAX/2.f) - 1.f;
	return rotation + r*accuracy;
}

void Weapon::update(float dt)
{
	timer += dt;
	timerAlt += dt;
}