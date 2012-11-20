#include "Slinker.h"


Slinker::Slinker(Pim::SpriteBatchNode *b, Pim::Vec2 pos)
	: Troll(b, pos)
{
	rect = Pim::Rect(0,20,15,10);
	
	createCircularBody(6, TROLLS, PLAYER | GROUND);
}
Slinker::~Slinker(void)
{
}
