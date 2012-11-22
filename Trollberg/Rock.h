#pragma once
#include "entity.h"

// Forward declarations
class Tumbler;
class Player;

class Rock : public Entity
{
public:
	Rock(Pim::SpriteBatchNode *actorSheet, Tumbler *tumbler, float dist, Player *p);

	// Create a circular body with custom physics attributes
	void createCircularBody(float pixelRadius, int category, int mask, float density=0.f);

	void update(float dt);

private:
	Player *player;

	float lifetime;

	bool  dangerous;
};

