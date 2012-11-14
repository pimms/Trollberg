#pragma once
#include "Trollberg.h"

class ParallaxLayer : public Pim::Layer
{
public:
	// Give the parallaxlayer two arguments: the layer it will follow, and the
	// factor of scrolling. A factor of 1 will make the two layers move equally.
	// A factor of 0.5 will make the parallaxlayer move at half the speed. A factor
	// of 2 will make the parallaxlayer move twice as fast.
	ParallaxLayer(Pim::Layer *follow, float factor);

	void loadResources();
	void update(float dt);

	// Provide a batch node and a rectangle from which the ParallaxLayer
	// will create a new sprite, and start acting parallaxy.
	void setSpriteInformation(Pim::SpriteBatchNode *batch, Pim::Rect rect);

private:
	Pim::Layer	*target;
	float		movementFactor;
};

