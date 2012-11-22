#include "Tumbler.h"
#include "TumblerAI.h"
#include "Player.h"
#include "Animation.h"

#define TL_TIMETODIE 1.4f

Tumbler::Tumbler(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p)
	: Troll(b, p)
{
	createRectangularBody(Pim::Vec2(14,26), TROLLS, TROLLS | PLAYER | GROUND | SENSOR);

	throwAnim.firstFramePos		= Pim::Vec2(0.f, 34.f);
	throwAnim.frameWidth		= 30;
	throwAnim.frameHeight		= 30;
	throwAnim.frameTime			= 0.3f;
	throwAnim.horizontalFrames	= 4;
	throwAnim.framesInAnimation = 4;
	throwAnim.totalFrames		= 4;

	walkAnim.firstFramePos		= Pim::Vec2(120.f, 34.f);
	walkAnim.frameWidth			= 30;
	walkAnim.frameHeight		= 30;
	walkAnim.frameTime			= 0.3f;
	walkAnim.horizontalFrames	= 4;
	walkAnim.framesInAnimation	= 4;
	walkAnim.totalFrames		= 4;

	deathAnim.firstFramePos		= Pim::Vec2(240.f, 34.f);
	deathAnim.frameWidth		= 30;
	deathAnim.frameHeight		= 30;
	deathAnim.frameTime			= 0.2f;
	deathAnim.horizontalFrames	= 7;
	deathAnim.framesInAnimation = 7;
	deathAnim.totalFrames		= 7;

	walkSpeed					= 5.f;
	health						= 100;
	ai							= new TumblerAI(this, pl);
	dead						= false;
	deathTimer					= 0.f;

	rect						= walkAnim.frameIndex(0);
}
Tumbler::~Tumbler(void)
{
}

void Tumbler::update(float dt)
{
	if (!dead)
	{
		ai->update(dt);
	}
	else
	{
		rect		= deathAnim.update(dt);
		deathTimer += dt;

		if (deathTimer >= TL_TIMETODIE)
		{
			deleteBody();
			parent->removeChild(this, true);
		}
	}
}

void Tumbler::takeDamage(int damage)
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