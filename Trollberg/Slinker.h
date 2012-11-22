#pragma once
#include "troll.h"

// Forward declarations
class SlinkerAI;
class Player;

class Slinker : public Troll
{
public:
	Slinker(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);
	~Slinker(void);

	void update(float dt);
	void takeDamage(int damage);

protected:
	friend class SlinkerAI;

	Animation				walkAnim;
	Animation				deathAnim;
};

