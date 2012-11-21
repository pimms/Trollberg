#pragma once

#include "Entity.h"

// Forward declarations
class LightRifle;
class Troll;
struct Pim::PreloadLightDef;

class LRBullet : public Entity
{
public:
	LRBullet(LightRifle *lr, Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle);
	void createLight();

	void update(float dt);

	bool hasDamaged(Troll *t);
	void damageTroll(Troll *t);

	float			life;
	b2Vec2			vel;

private:
	std::vector<Troll*>		damagedTrolls;
	LightRifle				*lightRifle;

	Pim::PreloadLightDef	*lightDef;
};