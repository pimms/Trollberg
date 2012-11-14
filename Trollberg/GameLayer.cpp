#include "GameLayer.h"


void GameLayer::loadResources()
{
	createLightingSystem(Pim::Vec2(1920.f, 1080.f));
	setLightingUnlitColor(Pim::Color(0.f, 0.f, 0.f, 0.8f));
}

void GameLayer::setSpriteInformation(Pim::SpriteBatchNode *b, Pim::Rect r)
{
	ground = new Pim::Sprite;
	ground->useBatchNode(b);
	ground->anchor = Pim::Vec2(0.f, 0.f);
	ground->rect = r;
	ground->position.x = 200;
	addChild(ground);
}