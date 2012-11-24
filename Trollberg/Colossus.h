#pragma once
#include "troll.h"

// Forward declarations
class ColossusAI;
class Player;

class Colossus : public Troll
{
public:
	Colossus(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);
	~Colossus();

	void createPhysics();

	void update(float dt);

private:
	friend class ColossusAI;

	b2Body		*body2;

	Animation	walkAnim;
	Animation	crushAnim;
	Animation	deathAnim;

	float		deathTimer;
	bool		dead;
};

