#include "Rock.h"
#include "Tumbler.h"
#include "Player.h"

Rock::Rock(Pim::SpriteBatchNode *actorSheet, Tumbler *tumbler, float dist, Player *p)
{
	listenFrame();
	lifetime	= 0.f;
	rect		= Pim::Rect(450,34,10,8);
	dangerous	= true;
	player		= p;

	useBatchNode(actorSheet);
	
	body = createCircularBody(4, TROLLS, PLAYER | GROUND);

	// Set the position of the rock
	b2Vec2 origin = tumbler->body->GetPosition();
	
	// The Tumbler sprite is facing left by default. Negative it.
	origin += b2Vec2(-0.8f*tumbler->scale.x, 0.1f);

	body->SetTransform(origin, 0.f);

	// Apply force to the rock
	b2Vec2 force(-3*dist*tumbler->scale.x, 1000);
	body->ApplyForce(force, tumbler->body->GetPosition());
}

b2Body* Rock::createCircularBody(float radius, int category, int mask, float density)
{
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.fixedRotation		= false;
	bd.allowSleep			= false;
	bd.position				= toB2(position);
	
	b2CircleShape shape;
	shape.m_radius			= radius / PTMR;

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.1f;
	fd.friction				= 0.9f;
	fd.density				= (density)?(density):(pow((radius/PTMR)*M_PI, 2));
	fd.userData				= this;
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	b2Body *retBody = world->CreateBody(&bd);
	retBody->CreateFixture(&fd);
	retBody->SetUserData(this);

	retBody->SetTransform(toB2(position), 0.f);

	return retBody;
}

void Rock::update(float dt)
{
	lifetime += dt;
	
	if (lifetime >= 5.f)
	{
		color.a = 1.f-(lifetime-5.f)/2.f;

		if (lifetime >= 7.f)
		{
			deleteBody();
			parent->removeChild(this, true);
			return;
		}
	}

	// The rock is only dangerous if it has yet to hit the ground
	if (dangerous)
	{
		for (auto c=body->GetContactList(); c; c=c->next)
		{
			if (otherCollidingFixture(c->contact, GROUND))
			{
				dangerous  = false;
			}

			if (otherCollidingFixture(c->contact, PLAYER))
			{
				player->takeDamage(1);
				dangerous = false;
			}
		}
	}
}