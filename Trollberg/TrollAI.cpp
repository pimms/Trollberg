#include "TrollAI.h"
#include "Player.h"
#include "Troll.h"


TrollAI::TrollAI()
{
	troll		= NULL;
	player		= NULL;
}

void TrollAI::moveToPlayer()
{
	float f = (trollPlayerXDiff()>0.f) ? (1.f) : (-1.f);
	
	b2Vec2 vel(f*troll->walkSpeed, troll->body->GetLinearVelocity().y);
	troll->body->SetLinearVelocity(vel);
}
void TrollAI::moveFromPlayer()
{
	float f = (trollPlayerXDiff()>0.f) ? (-1.f) : (1.f);
	
	b2Vec2 vel(f*troll->walkSpeed, troll->body->GetLinearVelocity().y);
	troll->body->SetLinearVelocity(vel);
}

void TrollAI::facePlayer()
{
	if (trollPlayerXDiff() > 0)
	{
		troll->scale.x = -1.f;
	}
	else
	{
		troll->scale.x = 1.f;
	}
}
void TrollAI::faceAwayPlayer()
{
	if (trollPlayerXDiff() > 0)
	{
		troll->scale.x = 1.f;
	}
	else
	{
		troll->scale.x = -1.f;
	}
}
float TrollAI::trollPlayerXDiff()
{
	return player->position.x - troll->position.x;
}

void TrollAI::damagePlayer()
{
	
}