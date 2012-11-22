#pragma once
#include "Pim.h"


class PauseLayer : public Pim::Layer
{
public:
	PauseLayer();
	~PauseLayer();

	void loadResources();

	void update(float dt);
	void keyEvent(Pim::KeyEvent &evt);

private:
	// Keep track of the paused time in order to properly track the
	// key events.
	float timer;
};

