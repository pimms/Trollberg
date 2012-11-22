#include "Slinker.h"
#include "SlinkerAI.h"
#include "Animation.h"

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

	rect		= Pim::Rect(0,14,20,20);
	ai			= new SlinkerAI(this, pl);
	walkSpeed	= 8.f;
	health		= 60.f;
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
	updateFloatLabels(dt);
	ai->update(dt);
}
void Slinker::takeDamage(int damage)
{
	Troll::takeDamage(damage);

	if (health <= 0)
	{
		deleteBody();
		parent->removeChild(this, true);
	}
}