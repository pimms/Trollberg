#include "Trollberg.h"

#include "GameLayer.h"
#include "TrollControl.h"
#include "Player.h"

#include "Troll.h"
#include "Slinker.h"

TrollControl::TrollControl(Pim::SpriteBatchNode *batch, GameLayer *l, 
						   Player *pl, float lvlWidth, int lvl)
{
	actorSheet	= batch;
	player		= pl;
	levelWidth	= lvlWidth;
	levelNum	= lvl;
	layer		= l;

	Slinker *sl = new Slinker(actorSheet, Pim::Vec2(1050.f, 50.f));
	layer->addChild(sl);
}
TrollControl::~TrollControl(void)
{
}
