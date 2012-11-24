#pragma once
#include "troll.h"

// Forward declarations
class Player;
class TumblerAI;

class Tumbler :public Troll
{
public:
	Tumbler(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);

	void createPhysics();

	void throwRock();

private:
	friend class TumblerAI;

	Player		*player;
};