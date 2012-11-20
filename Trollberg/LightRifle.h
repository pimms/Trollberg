#pragma once

#include "Weapon.h"

class LRBullet;
class LightRifle : public Weapon
{
public:
	LightRifle(Pim::SpriteBatchNode *batch);

	void fire();
	void altFire();
	void setMirrored(bool flag);

	void update(float dt);

private:
	std::list<LRBullet*> bullets;
};
class LRBullet : public Entity
{
public:
	LRBullet(Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle);
	void update(float dt);

	float			life;
	b2Vec2			vel;
};