#pragma once
#include "trollai.h"

// Forward declarations
class Colossus;

class ColossusAI : public TrollAI
{
public:
	ColossusAI(Colossus *troll, Player *pl);
	~ColossusAI();

	void update(float dt);
	void updateCrush(float dt);

private:
	Colossus		*colossus;
};

