#include "TumblerAI.h"
#include "Tumbler.h"
#include "Player.h"
#include "GameLayer.h"

#define TL_THROWTIME	0.75f	// Time required to throw
#define TL_THROWCD		0.f		// Cooldown between throws

TumblerAI::TumblerAI(Tumbler *tr, Player *pl)
{
	tumbler			= tr;
	troll			= tr;
	player			= pl;
	throwTimer		= 0.f;
	throwCooldown	= 0.f;
	isThrowing		= false;
	hasThrown		= false;
}
TumblerAI::~TumblerAI(void)
{
}

void TumblerAI::update(float dt)
{
	throwCooldown -= dt;

	// Is the player dead?
	if (GameLayer::isPlayerDead())
	{
		// If the tumbler is grounded, run away
		if (troll->isGrounded())
		{
			moveFromPlayer();
			tumbler->rect = tumbler->walkAnim.update(dt);
		}

		// Wait for the ground to hit it's feet
		else
		{
			tumbler->rect = tumbler->walkAnim.reset();
		}
		
		faceAwayPlayer();
		return;
	}

	// The throwing animation is running
	if (isThrowing)
	{
		tumbler->rect = tumbler->attackAnim.update(dt);
		throwTimer += dt;

		// Are we at the throw time? 
		if (throwTimer >= TL_THROWTIME)
		{
			tumbler->rect = tumbler->attackAnim.update(dt);

			// Has the rock been thrown yet?
			if (!hasThrown)
			{
				tumbler->throwRock();
				hasThrown = true;
			}
			
			// Wait for the animation to finish
			if (throwTimer >= TL_THROWTIME + 0.25f)
			{
				tumbler->rect = tumbler->walkAnim.reset();

				hasThrown		= false;
				isThrowing		= false;
				throwTimer		= 0.f;
				throwCooldown	= TL_THROWCD;
			}
		}
	}
	else	// Prepare to attack
	{
		if (troll->isGrounded())
		{
			// Move closer if required
			if (abs(trollPlayerXDiff() > 180))
			{
				tumbler->rect = tumbler->walkAnim.update(dt);

				moveToPlayer();
				facePlayer();
			}

			// Hurl that boulder!
			else if (throwCooldown <= 0.f)
			{
				tumbler->rect = tumbler->attackAnim.reset();

				throwCooldown	= 0.f;
				throwTimer		= 0.f;
				isThrowing		= true;

				facePlayer();
			}

			// He's got a towel! Run away!
			else if (trollPlayerXDiff() < 10.f)
			{
				moveFromPlayer();
				faceAwayPlayer();
			}

			// stand completely still
			else
			{
				troll->body->SetLinearVelocity(b2Vec2_zero);

				tumbler->rect = tumbler->walkAnim.frameIndex(0);
				tumbler->walkAnim.reset();
			}
		}

		// Wait for the troll to land
		else
		{
			tumbler->rect = tumbler->attackAnim.frameIndex(3);
			tumbler->walkAnim.reset();
		}
	}
}