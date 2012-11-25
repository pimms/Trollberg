#include "HUDLayer.h"
#include "Troll.h"
#include "GameLayer.h"


// The lifetime of the floating labels
#define FL_LIFETIME 0.5f


HUDLayer* HUDLayer::singleton = NULL;

HUDLayer* HUDLayer::getSingleton()
{
	return singleton;
}

HUDLayer::HUDLayer()
{
	singleton = this;
	immovable = true;

	isFading  = true;
	fadeTimer = 0.f;
}
HUDLayer::~HUDLayer()
{
	singleton = NULL;
}

void HUDLayer::loadResources()
{
	listenFrame();

	Pim::SpriteBatchNode *actor = GameLayer::getActorSheet();


	for (int i=0; i<3; i++)
	{
		hearts[i] = new Pim::Sprite;
		hearts[i]->useBatchNode(actor);
		hearts[i]->rect = Pim::Rect(126,0,9,9);
		hearts[i]->position = Pim::Vec2(20+(14*i), 196);
		addChild(hearts[i]);
	}

	// Load the fade sprite
	fadeSprite = new Pim::Sprite;
	fadeSprite->useBatchNode(actor);
	fadeSprite->scale = Pim::Vec2(384.f, 216.f);
	fadeSprite->anchor = Pim::Vec2(0.f,0.f);
	fadeSprite->rect = Pim::Rect(3,4,1,1);
	addChild(fadeSprite);


	FPSLabel = new Pim::Label(Troll::getFont());
	FPSLabel->color = Pim::Color(1.f, 1.f, 0.f, 1.f);
	FPSLabel->setTextAlignment(Pim::Label::TEXT_RIGHT);
	FPSLabel->position = Pim::Vec2(384.f, 20.f);
	FPSLabel->scale = Pim::Vec2(0.2f, 0.2f);
	addChild(FPSLabel);
}

void HUDLayer::update(float dt)
{
	updateLabels(dt);

	if (isFading)
	{
		fadeTimer += dt;
		fadeSprite->color.a = 1.f - fadeTimer / 2.f;

		if (fadeTimer >= 2.f)
		{
			isFading = false;
			fadeTimer = 0.f;

			removeChild(fadeSprite, true);
		}
	}

	std::stringstream ss;
	ss << 1.f/dt;
	FPSLabel->setTextWithFormat("FPS: %0.1f\n", 1.f/dt);
}

void HUDLayer::updateLabels(float dt)
{
	for (unsigned int i=0; i<labels.size(); i++)
	{
		labels[i]->lifetime		+= dt;
		labels[i]->color.a		-= dt / FL_LIFETIME;

		// f(x) = 2*pow((x-0.1),3) + 0.2*sin((x+0.1)*6)
		// http://www.abhortsoft.hu/functionvisualizer/functionvisualizer.html
		float x = labels[i]->lifetime;
		float fx = 2.f*pow((x-0.1f),3.f) + 0.2f*sinf((x+0.1f)*6.f);
		labels[i]->scale = Pim::Vec2(1.f, 1.f) * fx;

		//labels[i]->position		= position + getParentLayer()->position;
		labels[i]->position.x	 = labels[i]->initialX + GameLayer::getSingleton()->position.x;
		labels[i]->position.y	+= dt * 60.f;

		if (labels[i]->lifetime >= FL_LIFETIME)
		{
			removeChild(labels[i], true);
			labels.erase(labels.begin() + i);
		}
	}
}
void HUDLayer::addFloatLabel(FloatLabel *lab)
{
	addChild(lab);
	labels.push_back(lab);
}

void HUDLayer::setPlayerHealth(int health)
{
	for (int i=0; i<3; i++)
	{
		hearts[i]->hidden = (i >= health);
	}
}