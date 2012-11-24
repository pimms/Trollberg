#pragma once
#include "troll.h"

// Forward declarations
class SlinkerAI;
class Player;

class Slinker : public Troll
{
public:
	Slinker(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);

protected:
	friend class SlinkerAI;
};

