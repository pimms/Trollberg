#pragma once

#include "Entity.h"

// Forward declarations
class Weapon;
class Troll;
struct Pim::PreloadLightDef;

class Bullet : public Entity
{
public:
	Bullet(Weapon *lr, Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle);
	void createLight();

	void update(float dt);

	bool hasDamaged(Troll *t);
	void damageTroll(Troll *t);


	bool			dead;			// Triggered when the bullet hits the ground
	float			fadeOutTimer;	// Track the time to fade the bullet's size to 0

	float			angle;

	float			life;			// The current lifetime (in seconds) of the bullet
	float			maxLife;		// The maximally allowed lifetime
	b2Vec2			vel;			// The constant velocity of the bullet when alive

	std::vector<Troll*>		damagedTrolls;
	Weapon					*weapon;

	Pim::PreloadLightDef	*lightDef;
	int				finalLightRadius;
};