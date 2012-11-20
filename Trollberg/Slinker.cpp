#include "Slinker.h"
#include "SlinkerAI.h"


Slinker::Slinker(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 pos)
	: Troll(b, pos)
{
	rect = Pim::Rect(0,20,15,10);
	
	createCircularBody(6, TROLLS, PLAYER | GROUND);

	ai = new SlinkerAI(this, pl);

	walkSpeed = 8.f;
}
Slinker::~Slinker(void)
{
	if (ai) delete ai;
}

void Slinker::update(float dt)
{
	ai->update(dt);
}