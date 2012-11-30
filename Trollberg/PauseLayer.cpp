#include "PauseLayer.h"
#include "GameScene.h"
#include "MainMenuScene.h"

PauseLayer::PauseLayer()
{
	timer = 0.f;
}
PauseLayer::~PauseLayer()
{
}

void PauseLayer::loadResources()
{
	Pim::Sprite *pause = new Pim::Sprite("res\\pause.png");
	pause->anchor = Pim::Vec2(0.f, 0.f);
	addChild(pause);
}

void PauseLayer::update(float dt)
{
	timer += dt;
}
void PauseLayer::keyEvent(Pim::KeyEvent &evt)
{
	if (timer >= 0.1f && evt.isKeyFresh(Pim::KeyEvent::K_ESC))
	{
		Pim::GameControl::getSingleton()->unpause();
	}

	// The timer is required to allow for any eventual played sounds to
	// finish and delete themselves. Last restort quick-fix. Nasty fix.
	if (timer >= 1.5f)	
	{
		if (evt.isKeyFresh(Pim::KeyEvent::K_R))
		{
			Pim::GameControl::getSingleton()->unpause();
			GameScene::getSingleton()->restart();
		}
		else if (evt.isKeyFresh(Pim::KeyEvent::K_M))
		{
			Pim::GameControl::getSingleton()->unpause();
			Pim::GameControl::getSingleton()->setScene(new MainMenuScene);
		}
	}
}