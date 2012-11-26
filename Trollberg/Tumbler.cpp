#pragma once
#include "Tumbler.h"
#include "TumblerAI.h"
#include "Player.h"
#include "Animation.h"
#include "GameLayer.h"
#include "Rock.h"
#include "TrollControl.h"

#define TL_TIMETODIE 1.4f
#define TL_FADETIME 6.f

Tumbler::Tumbler(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p)
	: Troll(b, p)
{
	createPhysics();

	attackAnim.firstFramePos	= Pim::Vec2(0.f, 34.f);
	attackAnim.frameWidth		= 30;
	attackAnim.frameHeight		= 30;
	attackAnim.frameTime		= 0.25f;
	attackAnim.horizontalFrames	= 4;
	attackAnim.framesInAnimation = 4;
	attackAnim.totalFrames		= 4;

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

	scoreValue					= 75;
	player						= pl;
	walkSpeed					= 5.f;
	health						= 100;
	ai							= new TumblerAI(this, player);
	b2offset					= Pim::Vec2(0.f, 4.f);

	timeToDie					= 1.4f;

	rect						= walkAnim.frameIndex(0);
}

void Tumbler::createPhysics()
{
	body = createCircularBody(7, TROLLS_TL, TROLLS_TL | PLAYER | GROUND | SENSOR);
	sensor = createSensor(body, -8.f/PTMR);

	body2 = createCircularBody(7, TROLLS_TL, TROLLS_TL | PLAYER | GROUND | SENSOR);

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

void Tumbler::throwRock()
{
	Rock *rock = new Rock(actorSheet, this, ai->trollPlayerXDiff(), player);
	GameLayer::getSingleton()->addChild(rock);
}
