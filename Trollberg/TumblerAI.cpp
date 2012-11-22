#include "TumblerAI.h"
#include "Tumbler.h"
#include "Player.h"
#include "GameLayer.h"

TumblerAI::TumblerAI(Tumbler *tr, Player *pl)
{
	tumbler		= tr;
	troll		= tr;
	player		= pl;
}
TumblerAI::~TumblerAI(void)
{
}

void TumblerAI::update(float dt)
{
	if (GameLayer::isPlayerDead())
	{
		if (troll->isGrounded())
		{
			moveFromPlayer();
			tumbler->rect = tumbler->walkAnim.update(dt);
		}
		else
		{
			tumbler->rect = tumbler->walkAnim.frameIndex(4);
		}
		
		faceAwayPlayer();
		return;
	}

	if (troll->isGrounded())
	{
		if ( abs(trollPlayerXDiff()) < 50  )
		{
			moveFromPlayer();
			faceAwayPlayer();
		}
		else if ( abs(trollPlayerXDiff()) > 120 )
		{
			moveToPlayer();
			facePlayer();
		}
		else
		{
			// Throw rock
		}
	}
	else
	{
		troll->body->SetLinearVelocity(b2Vec2(0, troll->body->GetLinearVelocity().y) );
	}
}