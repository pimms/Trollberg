#include "ColossusAI.h"
#include "Colossus.h"
#include "GameLayer.h"


ColossusAI::ColossusAI(Colossus *t, Player *pl)
{
	troll		= t;
	colossus	= t;
	player		= pl;

}
ColossusAI::~ColossusAI()
{
}

void ColossusAI::update(float dt)
{
	// Is the player dead?
	if (GameLayer::isPlayerDead())
	{
		// If the tumbler is grounded, run away
		if (troll->isGrounded())
		{
			moveFromPlayer();
			colossus->rect = colossus->walkAnim.update(dt);
		}

		// Wait for the ground to hit it's feet
		else
		{
			colossus->rect = colossus->walkAnim.reset();
		}
		
		faceAwayPlayer();
		return;
	}

	if (abs(trollPlayerXDiff()) > 20.f)
	{
		moveToPlayer();
		facePlayer();

		colossus->rect = colossus->walkAnim.update(dt);
	}
	else
	{
		updateCrush(dt);
	}
}

void ColossusAI::updateCrush(float dt)
{
	troll->body->SetLinearVelocity(b2Vec2_zero);
}