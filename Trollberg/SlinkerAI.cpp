#include "SlinkerAI.h"
#include "Troll.h"
#include "Player.h"

SlinkerAI::SlinkerAI(Troll *t, Player *p)
{
	troll				= t;
	player				= p;
	leapTimer			= 0.f;

	hasDamagedPlayer	= false;
	isLeaping			= false;
	willLeap			= false;
}

void SlinkerAI::update(float dt)
{
	stompCheck();

	if (isLeaping || willLeap)
	{
		leapUpdate(dt);
	}
	else if (abs(trollPlayerXDiff()) >= 100.f)
	{
		moveToPlayer();
	}
	else
	{
		willLeap = true;
		troll->body->SetLinearVelocity(b2Vec2_zero);
	}
}
void SlinkerAI::leapUpdate(float dt)
{
	leapTimer += dt;

	if (!isLeaping && troll->isGrounded())
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
			// Is the troll currently touching the player?
			if (!hasDamagedPlayer)
			{
				b2Fixture *playerFix = troll->otherCollidingFixture(c->contact, PLAYER);
				if (playerFix)
				{
					float d = player->position.y - troll->position.y;
					if (d <= 0.f)
					{
						player->takeDamage(1);
						hasDamagedPlayer = true;
					}
				}
			}

			if (troll->otherCollidingFixture(c->contact, GROUND))
			{
				hasDamagedPlayer = false;
				isLeaping = false;
				leapTimer = 0.f;
				return;
			}
		}
	}
}
void SlinkerAI::stompCheck()
{
	for (auto c=troll->body->GetContactList(); c; c=c->next)
	{
		b2Fixture *playerFix = troll->otherCollidingFixture(c->contact, PLAYER);
		if (playerFix && troll->isGrounded())
		{
			float d = player->position.y - troll->position.y;
			if (d >= 0.f)
			{
				troll->takeDamage(100);
			}
		}
	}
}