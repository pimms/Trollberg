#pragma once
#include "actor.h"

// Forward declarations
class TrollAI;

class Troll : public Actor
{
public:
	Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p);

	virtual void update(float dt) = 0;

protected:
	friend class TrollAI;
	TrollAI		*ai;

	float		walkSpeed;
};

