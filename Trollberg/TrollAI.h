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

	virtual void moveToPlayer();
	virtual void moveFromPlayer();

	void facePlayer();
	void faceAwayPlayer();

	float trollPlayerXDiff();

	void damagePlayer();

protected:

	Troll	*troll;
	Player	*player;
};

