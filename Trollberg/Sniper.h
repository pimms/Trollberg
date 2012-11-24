#pragma once
#include "weapon.h"

class Bullet;

class Sniper : public Weapon
{
public:
	Sniper(Pim::SpriteBatchNode *b);

	void fire();
	void altFire();	 
	void setMirrored(bool flag);
	Pim::Vec2 muzzlePoint();

	float damage(Bullet*);
};

