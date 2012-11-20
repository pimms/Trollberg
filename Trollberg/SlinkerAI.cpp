#include "SlinkerAI.h"
#include "Troll.h"
#include "Player.h"

SlinkerAI::SlinkerAI(Troll *t, Player *p)
{
	troll	= t;
	player	= p;
	leapTimer = 0.f;
	leapCooldown = 0.f;

	isLeaping	= false;
	willLeap	= false;
}

void SlinkerAI::update(float dt)
{
	leapCooldown -= dt;

	if ((isLeaping || willLeap))
		leapUpdate(dt);
	else if (abs(trollPlayerXDiff()) >= 100.f)
		moveToPlayer();
	else
	{
		willLeap = true;
		troll->body->SetLinearVelocity(b2Vec2_zero);
	}
}
void SlinkerAI::leapUpdate(float dt)
{
	leapCooldown = 4.f;
	leapTimer += dt;

	if (!isLeaping)
	{
		willLeap = false;
		isLeaping = true;

		b2Vec2 force(trollPlayerXDiff()/2.f, 50);
		if (force.y < 10) force.y = 10;

		troll->body->ApplyLinearImpulse(force, troll->body->GetPosition());
	}
	else if (leapTimer >= 0.5f)
	{
		for (auto c=troll->body->GetContactList(); c; c=c->next)
		{
			if (c->contact->IsTouching())
			{
				isLeaping = false;
				leapTimer = 0.f;
			}
		}
	}
}