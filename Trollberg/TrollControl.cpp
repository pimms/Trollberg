#include "Trollberg.h"

#include "Weapon.h"
#include "GameLayer.h"
#include "TrollControl.h"
#include "Player.h"

#include "Troll.h"
#include "Slinker.h"
#include "Tumbler.h"

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
		Tumbler *tl = new Tumbler(pl, actorSheet, Pim::Vec2(500+50*i, 150.f));
		layer->addChild(tl);
	}
}
TrollControl::~TrollControl(void)
{
	singleton = NULL;
}
