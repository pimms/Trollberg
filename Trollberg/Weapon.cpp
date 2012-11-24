#include "Weapon.h"
#include "GameScene.h"
#include "Bullet.h"

#include "Revolver.h"
#include "Shotgun.h"
#include "Sniper.h"

Weapon* Weapon::createWeapon(Pim::SpriteBatchNode *b, WeaponID wid)
{
	switch (wid)
	{
		case REVOLVER:
			return new Revolver(b);

		case SHOTGUN:
			return new Shotgun(b);

		case SNIPER:
			return new Sniper(b);	
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

float Weapon::damage(Bullet *bullet)
{
	// Get a random number between 0 and 1
	float r = (float)rand()/(float)RAND_MAX;
	return minDamage + r*(maxDamage-minDamage+1);
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
}