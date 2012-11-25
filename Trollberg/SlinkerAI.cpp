#include "SlinkerAI.h"
#include "Troll.h"
#include "Player.h"
#include "Slinker.h"
#include "GameLayer.h"

SlinkerAI::SlinkerAI(Slinker *t, Player *p)
{
	troll				= t;
	slinker				= t;
	player				= p;
	leapTimer			= 0.f;

	hasDamagedPlayer	= false;
	isLeaping			= false;
	willLeap			= false;
}

void SlinkerAI::update(float dt)
{
	if (GameLayer::isPlayerDead())
	{
		if (troll->isGrounded())
		{
			moveFromPlayer();
			slinker->rect = slinker->walkAnim.update(dt);
		}
		else
		{
			slinker->rect = slinker->walkAnim.frameIndex(4);
		}

		faceAwayPlayer();

		return;
	}

	stompCheck();

	if (isLeaping || willLeap)
	{
		leapUpdate(dt);
		slinker->walkAnim.reset();

		if (troll->isGrounded())
		{
			slinker->rect = slinker->walkAnim.frameIndex(0);
		}
		else
		{
			slinker->rect = slinker->walkAnim.frameIndex(4);
		}
	}
	else if (abs(trollPlayerXDiff()) >= 60 + rand()%81)
	{
		moveToPlayer();
		facePlayer();

		slinker->rect = slinker->walkAnim.update(dt);
	}
	else
	{
		willLeap = true;
		troll->body->SetLinearVelocity(b2Vec2_zero);

		facePlayer();
	}
}
void SlinkerAI::leapUpdate(float dt)
{
	leapTimer += dt;

	if (!isLeaping && troll->isGrounded())
	{
		willLeap = false;
		isLeaping = true;

		// Calculate a force for the X axis
		float xForce = trollPlayerXDiff() / 2.f;
		//FIX, divided by zero patch
		if((int)trollPlayerXDiff() == 0)
		{
			return;
		}
		int ran = rand() % abs((int)trollPlayerXDiff());
		xForce += ran * (xForce > 0) ? (1.f) : (-1.f);

		b2Vec2 force = b2Vec2(xForce, 50 + rand() % 45);

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
				return;
			}
		}
	}
}