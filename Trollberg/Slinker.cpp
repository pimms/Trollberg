#include "Slinker.h"
#include "SlinkerAI.h"


Slinker::Slinker(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 pos)
	: Troll(b, pos)
{
	createCircularBody(6, TROLLS, PLAYER | GROUND | SENSOR);

	rect		= Pim::Rect(0,20,15,10);
	ai			= new SlinkerAI(this, pl);
	walkSpeed	= 8.f;
	health		= 60.f;
}
Slinker::~Slinker(void)
{
	if (ai) delete ai;
}

void Slinker::update(float dt)
{
	updateFloatLabels(dt);
	ai->update(dt);
}