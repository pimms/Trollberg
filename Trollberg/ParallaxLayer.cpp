#include "ParallaxLayer.h"


ParallaxLayer::ParallaxLayer(Pim::Layer *follow, float factor)
{
	target = follow;
	movementFactor = factor;
}

void ParallaxLayer::loadResources()
{
	listenFrame();
}
void ParallaxLayer::update(float dt)
{
	if (target)
		position = target->position * movementFactor;
}

void ParallaxLayer::setSpriteInformation(Pim::SpriteBatchNode *b, Pim::Rect r)
{
	Pim::Sprite *s = new Pim::Sprite;
	s->useBatchNode(b);
	s->rect = r;
	s->anchor = Pim::Vec2(0.f, 0.f);
	addChild(s);
}