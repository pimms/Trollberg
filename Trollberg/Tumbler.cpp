#pragma once
#include "Tumbler.h"
#include "TumblerAI.h"
#include "Player.h"
#include "Animation.h"
#include "GameLayer.h"
#include "Rock.h"
#include "TrollControl.h"

#define TL_TIMETODIE 1.4f

Tumbler::Tumbler(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p)
	: Troll(b, p)
{
	createCircularBody(7, TROLLS, TROLLS | PLAYER | GROUND | SENSOR);

	throwAnim.firstFramePos		= Pim::Vec2(0.f, 34.f);
	throwAnim.frameWidth		= 30;
	throwAnim.frameHeight		= 30;
	throwAnim.frameTime			= 0.25f;
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

	player						= pl;
	walkSpeed					= 5.f;
	health						= 100;
	dead						= false;
	deathTimer					= 0.f;
	ai							= new TumblerAI(this, player);

	rect						= walkAnim.frameIndex(0);
}
Tumbler::~Tumbler(void)
{
	if (ai)
	{
		delete ai;
	}
}

void Tumbler::createCircularBody(float radius, int category, int mask, float density)
{
	// Create the lower body
	Actor::createCircularBody(radius, category, mask, density);

	// Create the upper body
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.fixedRotation		= true;
	bd.allowSleep			= false;
	bd.position				= toB2(position);
	
	b2CircleShape shape;
	shape.m_radius			= radius / PTMR;

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.f;
	fd.friction				= 0.f;
	fd.density				= (density)?(density):(pow((radius/PTMR)*M_PI, 2));
	fd.userData				= this;
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	body2 = world->CreateBody(&bd);
	body2->CreateFixture(&fd);
	body2->SetUserData(this);

	// Joint the two bodies together
	b2WeldJointDef jd;
	jd.bodyA = body;
	jd.bodyB = body2;

	jd.localAnchorB = b2Vec2(0, -radius*2);
	world->CreateJoint(&jd);
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

void Tumbler::throwRock()
{
	Rock *rock = new Rock(actorSheet, this, ai->trollPlayerXDiff(), player);
	GameLayer::getSingleton()->addChild(rock);
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

void Tumbler::deleteBody()
{
	Actor::deleteBody();

	if (body2)
	{
		world->DestroyBody(body2);
		body2 = NULL;
	}
}