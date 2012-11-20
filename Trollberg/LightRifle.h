#pragma once

#include "Weapon.h"

// Forward declarations
class LRBullet;

class LightRifle : public Weapon
{
public:
	LightRifle(Pim::SpriteBatchNode *batch);

	void removeChild(Pim::GameNode *n, bool cleanup=false);

	void fire();
	void altFire();
	void setMirrored(bool flag);

	void update(float dt);

private:
	std::list<LRBullet*> bullets;
};
