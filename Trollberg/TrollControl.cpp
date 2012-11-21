#include "Trollberg.h"

#include "Weapon.h"
#include "GameLayer.h"
#include "TrollControl.h"
#include "Player.h"

#include "Troll.h"
#include "Slinker.h"

TrollControl* TrollControl::singleton = NULL;

TrollControl::TrollControl(Pim::SpriteBatchNode *batch, GameLayer *l, 
						   Player *pl, float lvlWidth, int lvl)
{
	singleton = this;

	actorSheet	= batch;
	player		= pl;
	levelWidth	= lvlWidth;
	levelNum	= lvl;
	layer		= l;

	for (int i=0; i<5; i++)
	{
		Slinker *sl = new Slinker(pl, actorSheet, Pim::Vec2(500+50*i, 150.f));
		layer->addChild(sl);
	}
}
TrollControl::~TrollControl(void)
{
	singleton = NULL;
}
