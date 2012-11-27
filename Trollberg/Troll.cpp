#include "Troll.h"
#include "TrollAI.h"
#include "HUDLayer.h"
#include "TrollControl.h"



Troll::Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p)
	: Actor(n, p)
{
	walkSpeed	= NULL;
	ai			= NULL;
	body2		= NULL;

	deathTimer	= 0.f;
	dead		= false;
	isFading	= false;

	timeToDie	= 2.f;
	fadeTime	= 6.f;

	scoreValue  = 0;

	listenFrame();
}
Troll::~Troll()
{
	if (ai)
	{
		delete ai;
	}
}

void Troll::update(float dt)
{
	if (!dead)
	{
		ai->update(dt);
	}
	else
	{
		deathTimer += dt;
		
		if (!isFading)
		{
			if (deathAnim.curFrame != deathAnim.framesInAnimation-1)
			{
				rect = deathAnim.update(dt);
			}

			if (deathTimer >= timeToDie)
			{
				if (isGrounded())
				{
					deleteBody();

					isFading	= true;
					deathTimer	= 0.f;
					
					rect = deathAnim.frameIndex(deathAnim.framesInAnimation-1);
				}
			}
			else if (isGrounded())
			{
				deleteBody();
			}
		}
		else
		{
			if (deathTimer > fadeTime-1.f)
			{
				color.a = fadeTime - deathTimer;
				if (deathTimer >= fadeTime)
				{
					parent->removeChild(this, true);
				}
			}
		}
	}
}

void Troll::takeDamage(int damage)
{
	if (!dead)
	{
		health -= damage;
		HUDLayer::getSingleton()->addDamageLabel(this, damage);

		//Pim::Sound *s = new Pim::Sound;
		//s->useCache("splat0");
		//s->deleteWhenDone = true;
		//s->play();

		if (health <= 0)
		{
			// Flag as dead
			dead = true;

			// Notify TrollControl of my unfortunate and permanent departure
			TrollControl::getSingleton()->trollKilled();

			// Create a score label
			HUDLayer::getSingleton()->addScoreLabel(this, scoreValue);

			// Set the collision filter to only collide with the ground
			b2Filter filter;
			filter.categoryBits = TROLLS;
			filter.maskBits = GROUND;
			body->GetFixtureList()->SetFilterData(filter);
		}
	}
}

void Troll::deleteBody()
{
	Actor::deleteBody();

	if (body2)
	{
		world->DestroyBody(body2);
		body2 = NULL;
	}
}