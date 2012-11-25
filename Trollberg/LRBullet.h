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

private:
	friend class LightRifle;

	bool			dead;			// Triggered when the bullet hits the ground
	float			fadeOutTimer;	// Track the time to fade the bullet's size to 0

	float			angle;

	float			life;			// The current lifetime (in seconds) of the bullet
	b2Vec2			vel;			// The constant velocity of the bullet when alive

	std::vector<Troll*>		damagedTrolls;
	LightRifle				*lightRifle;

	int						finalLightRadius;
	Pim::PreloadLightDef	*lightDef;
};