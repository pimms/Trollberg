#pragma once
#include "TrollAI.h"

class SlinkerAI : public TrollAI
{
public:
	SlinkerAI(Troll *troll, Player *pl);

	void update(float dt);
	void leapUpdate(float dt);

protected:
	bool willLeap;
	bool isLeaping;
	bool hasDamagedPlayer;		// Has the Slinker damaged the player this leap?
	float leapTimer;
};

