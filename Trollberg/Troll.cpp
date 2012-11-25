#include "Troll.h"
#include "TrollAI.h"
#include "HUDLayer.h"
#include "TrollControl.h"

Pim::Font* Troll::floatFont = NULL;

void Troll::createFont()
{
	if (!floatFont)
	{
		floatFont = new Pim::Font("res\\arial.ttf", 40, false);
	}
}
Pim::Font* Troll::getFont()
{
	return floatFont;
}
void Troll::destroyFont()
{
	if (floatFont)
	{
		delete floatFont;
	}
}

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
				body->SetLinearVelocity(b2Vec2_zero);

				if (body2)
				{
					body2->SetLinearVelocity(b2Vec2_zero);
				}
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
		addFloatLabel(damage);

		if (health <= 0)
		{
			// Flag as dead
			dead = true;

			//tell troll controll i'm dead
			TrollControl::getSingleton()->trollKilled();
			// Set the collision filter to only collide with the ground
			b2Filter filter;
			filter.categoryBits = TROLLS;
			filter.maskBits = GROUND;
			body->GetFixtureList()->SetFilterData(filter);
		}
	}
}
void Troll::addFloatLabel(int damage)
{
	std::stringstream ss;
	ss << damage;

	FloatLabel *lab = new FloatLabel(floatFont);
	lab->setText(ss.str());
	lab->scale = Pim::Vec2(0.1f, 0.1f);
	lab->color = Pim::Color(1.f, 1.f, 0.f, 1.f);
	lab->position = position + getParentLayer()->position + Pim::Vec2(0.f, 15.f);
	lab->initialX = position.x;
	
	HUDLayer::getSingleton()->addFloatLabel(lab);
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