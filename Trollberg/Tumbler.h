#pragma once
#include "troll.h"

// Forward declarations
class Player;
class TumblerAI;

class Tumbler :public Troll
{
public:
	Tumbler(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);
	~Tumbler(void);

	void update(float dt);

	void takeDamage(int damage);

private:
	friend class TumblerAI;

	Animation	walkAnim;
	Animation	throwAnim;
	Animation	deathAnim;

	float		deathTimer;
	bool		dead;
};

