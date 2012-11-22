#include "Slinker.h"
#include "SlinkerAI.h"
#include "Animation.h"

#define SL_TIMETODIE 0.7f

Slinker::Slinker(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 pos)
	: Troll(b, pos)
{
	createCircularBody(10, TROLLS, TROLLS | PLAYER | GROUND | SENSOR, 1.13f);

	walkAnim.firstFramePos		= Pim::Vec2(0.f, 14.f);
	walkAnim.frameWidth			= 20;
	walkAnim.frameHeight		= 20;
	walkAnim.frameTime			= 0.1f;
	walkAnim.horizontalFrames	= 5;
	walkAnim.totalFrames		= 5;
	walkAnim.framesInAnimation  = 4;

	deathAnim.firstFramePos		= Pim::Vec2(100.f, 14.f);
	deathAnim.frameWidth		= 20;
	deathAnim.frameHeight		= 20;
	deathAnim.frameTime			= 0.1f;
	deathAnim.horizontalFrames	= 7;
	deathAnim.totalFrames		= 7;
	deathAnim.framesInAnimation = 7;

	deathTimer	= 0.f;
	dead		= false;
	ai			= new SlinkerAI(this, pl);
	walkSpeed	= 8.f;
	health		= 60.f;

	rect		= walkAnim.frameIndex(0);
}
Slinker::~Slinker(void)
{
	if (ai) 
	{	
		delete ai;
	}
}

void Slinker::update(float dt)
{
	if (!dead)
	{
		ai->update(dt);
	}
	else
	{
		rect		= deathAnim.update(dt);
		deathTimer += dt;

		if (deathTimer >= SL_TIMETODIE)
		{
			deleteBody();
			parent->removeChild(this, true);
		}
	}
}
void Slinker::takeDamage(int damage)
{
	if (!dead)
	{
		Troll::takeDamage(damage);

		if (health <= 0)
		{
			// Flag as dead
			dead = true;

			// Set the collision filter to only collide with the ground
			b2Filter filter;
			filter.categoryBits = TROLLS;
			filter.maskBits = GROUND;
			body->GetFixtureList()->SetFilterData(filter);
		}
	}
}