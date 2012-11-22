#include "HUDLayer.h"
#include "Troll.h"
#include "GameLayer.h"


HUDLayer* HUDLayer::singleton = NULL;

HUDLayer* HUDLayer::getSingleton()
{
	return singleton;
}

HUDLayer::HUDLayer()
{
	singleton = this;
	immovable = true;
	Troll::setHUDLayer(this);
}
HUDLayer::~HUDLayer()
{
	singleton = NULL;
}

void HUDLayer::loadResources()
{
	Pim::SpriteBatchNode *actor = GameLayer::getActorSheet();

	for (int i=0; i<3; i++)
	{
		hearts[i] = new Pim::Sprite;
		hearts[i]->useBatchNode(actor);
		hearts[i]->rect = Pim::Rect(126,0,9,9);
		hearts[i]->position = Pim::Vec2(20+(14*i), 196);
		addChild(hearts[i]);
	}
}

void HUDLayer::setPlayerHealth(int health)
{
	for (int i=0; i<3; i++)
	{
		hearts[i]->hidden = (i >= health);
	}
}