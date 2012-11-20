#pragma once

#include "Entity.h"

class LightRifle;
class Troll;

class LRBullet : public Entity
{
public:
	LRBullet(LightRifle *lr, Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle);
	void update(float dt);

	bool hasDamaged(Troll *t);
	void damageTroll(Troll *t);

	float			life;
	b2Vec2			vel;

private:
	std::vector<Troll*>		damagedTrolls;
	LightRifle				*lightRifle;
};