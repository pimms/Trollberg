#pragma once
#include "TrollAI.h"

// Forward declarations
class Slinker;

class SlinkerAI : public TrollAI
{
public:
	SlinkerAI(Slinker *troll, Player *pl);

	void update(float dt);
	void leapUpdate(float dt);
	void stompCheck();

protected:
	Slinker	*slinker;

	bool willLeap;
	bool isLeaping;
	bool hasDamagedPlayer;		// Has the Slinker damaged the player this leap?
	float leapTimer;
};

