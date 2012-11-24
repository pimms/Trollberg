#pragma once

#include "Weapon.h"

// Forward declarations
class Bullet;

class Revolver : public Weapon
{
public:
	Revolver(Pim::SpriteBatchNode *batch);

	void removeChild(Pim::GameNode *n, bool cleanup=false);

	void fire();
	void altFire();
	void setMirrored(bool flag);
	Pim::Vec2 muzzlePoint();

	void update(float dt);
};
