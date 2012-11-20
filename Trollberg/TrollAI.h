#pragma once

#include "Trollberg.h"

// Forward declarations
class Troll;
class Player;

class TrollAI
{
public:
	TrollAI();
	virtual void update(float dt) = 0;

	void moveToPlayer();
	float trollPlayerXDiff();

protected:

	Troll	*troll;
	Player	*player;
};

