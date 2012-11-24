#include "Trollberg.h"

#include "Weapon.h"
#include "GameLayer.h"
#include "TrollControl.h"
#include "Player.h"

#include "Troll.h"
#include "Slinker.h"
#include "Tumbler.h"
#include "Colossus.h"

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

	Slinker *sl = new Slinker(pl, actorSheet,  Pim::Vec2(600.f, 150.f));
	layer->addChild(sl);

	Tumbler *tl = new Tumbler(pl, actorSheet, Pim::Vec2(700.f, 150.f));
	layer->addChild(tl);

	Colossus *cl = new Colossus(pl, actorSheet, Pim::Vec2(500.f, 150.f));
	layer->addChild(cl);
}
TrollControl::~TrollControl(void)
{
	singleton = NULL;
}
