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
	createPhysics();

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

void Tumbler::createPhysics()
{
	body = createCircularBody(7, TROLLS, TROLLS | PLAYER | GROUND | SENSOR);
	sensor = createSensor(body, -8.f/PTMR);

	body2 = createCircularBody(7, TROLLS, TROLLS | PLAYER | GROUND | SENSOR);

	// Joint the two bodies together. WeldJoint didn't work. WHAT THE FLYING FUCK.
	// Using a revolute joint instead, turning off rotation.
	b2RevoluteJointDef jd;
	jd.Initialize(body, body2, body->GetPosition());
	jd.localAnchorB = b2Vec2(0,-0.7);
	jd.collideConnected = false;
	jd.lowerAngle = 0.f;
	jd.upperAngle = 0.f;

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