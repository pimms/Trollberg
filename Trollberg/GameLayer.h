#pragma once
#include "Trollberg.h"

class GameLayer : public Pim::Layer
{
public:
	void loadResources();

	// Provide the game layer with a batch node (containing the texture), and a 
	// rectangle which will be rendered from said texture.
	void setSpriteInformation(Pim::SpriteBatchNode *b, Pim::Rect r);

private:

	// The ground sprite. 
	Pim::Sprite		*ground;
};

