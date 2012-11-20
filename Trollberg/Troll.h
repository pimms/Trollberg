#pragma once
#include "actor.h"

// Forward declarations
class TrollAI;

class Troll : public Actor
{
public:
	Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p);

private:
	friend class TrollAI;
};

