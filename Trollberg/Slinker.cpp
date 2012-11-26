#pragma once
#include "Slinker.h"
#include "SlinkerAI.h"
#include "Animation.h"
#include "TrollControl.h"

#define SL_TIMETODIE 0.7f
#define SL_FADETIME 6.f

Slinker::Slinker(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 pos)
	: Troll(b, pos)
{
	body = createCircularBody(10, TROLLS, TROLLS | PLAYER | GROUND | SENSOR, 1.13f);
	sensor = createSensor(body, -8.f/PTMR);

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

	scoreValue	= 50;
	deathTimer	= 0.f;
	dead		= false;
	ai			= new SlinkerAI(this, pl);
	walkSpeed	= 8.f;
	health		= 60.f;

	timeToDie	= 1.4f;

	rect		= walkAnim.frameIndex(0);
}
