#include "Troll.h"
#include "TrollAI.h"

Troll::Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p)
	: Actor(n, p)
{
	walkSpeed	= NULL;
	ai			= NULL;

	listenFrame();
}
