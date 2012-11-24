#pragma once
#include "weapon.h"

// Forward declarations
class Bullet;

class Shotgun : public Weapon
{
public:
	Shotgun(Pim::SpriteBatchNode *batch);
	
	void fire();
	void altFire();	 
	void setMirrored(bool flag);
	Pim::Vec2 muzzlePoint();

	float damage(Bullet*);
};

