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

	actorSheet = GameLayer::getActorSheet();

	loadHelperSprites();
	loadWeaponCogs();
	loadPowerCogs();
	loadPowerBar();
	loadHearts();

	// Load the fade sprite
	fadeSprite = new Pim::Sprite;
	fadeSprite->useBatchNode(actorSheet);
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
void HUDLayer::loadHelperSprites()
{
	Pim::Sprite *menu = new Pim::Sprite;
	menu->useBatchNode(actorSheet);
	menu->anchor = Pim::Vec2(0.f, 1.f);
	menu->rect = Pim::Rect(120, 149, 46, 41);
	menu->position = Pim::Vec2(0.f, 216.f);
	addChild(menu);

	soundInd = new Pim::Sprite;
	soundInd->useBatchNode(actorSheet);
	soundInd->anchor = Pim::Vec2(0.f, 1.f);
	soundInd->rect = Pim::Rect(203, 149, 10, 24);
	soundInd->position = Pim::Vec2(58.f, 216.f);
	addChild(soundInd);
}
void HUDLayer::loadWeaponCogs()
{
	// Load the weapong cog
	weaponCog = new Pim::Sprite;
	weaponCog->useBatchNode(actorSheet);
	weaponCog->rect = Pim::Rect(395.f, 149.f, 58.f, 55.f);
	weaponCog->position = Pim::Vec2(192.f, 189.f);
	addChild(weaponCog);

	// Load the weapon sprites, attach them to the cog
	for (int i=0; i<3; i++)
	{
		float radian = (60 + 120*i)*DEGTORAD;
		Pim::Vec2 pos(16*cosf(radian), 16*sinf(radian));
		pos -= Pim::Vec2(5*cosf(radian-90*DEGTORAD), 5*sinf(radian-90*DEGTORAD));

		weapons[i] = new Pim::Sprite;
		weapons[i]->useBatchNode(actorSheet);
		weapons[i]->rect = Pim::Rect(0.f, 211 + 13*i, 30, 13);
		weapons[i]->position = pos;
		weapons[i]->rotation = (radian*RADTODEG);
		weaponCog->addChild(weapons[i]);
	}

	arrowCog = new Pim::Sprite;
	arrowCog->useBatchNode(actorSheet);
	arrowCog->rect = weaponCog->rect;//sPim::Rect(35.f, 149.f, 41.f, 40.f);
	arrowCog->position = weaponCog->position + Pim::Vec2(50.f, 24.f);
	addChild(arrowCog);

	// Load the arrow sprites, attach them to the cog
	for (int i=0; i<3; i++)
	{
		float radian = (30 + 120*i)*DEGTORAD;
		Pim::Vec2 pos(16*cosf(radian), 16*sinf(radian));

		Pim::Sprite *arrow = new Pim::Sprite;
		arrow->useBatchNode(actorSheet);
		arrow->rect = Pim::Rect(31.f, 223.f, 14.f, 10.f);
		arrow->position = pos;
		arrow->rotation = radian * RADTODEG;
		arrowCog->addChild(arrow);
	}
}
void HUDLayer::loadPowerCogs()
{
	Pim::Vec2 cogPos[3] = {
		Pim::Vec2(340.f, 202.f),
		Pim::Vec2(370.f, 179.f),
		Pim::Vec2(380.f, 216.f),
	};

	for (int i=0; i<3; i++)
	{
		powerCogs[i] = new Pim::Sprite;
		powerCogs[i]->useBatchNode(actorSheet);
		powerCogs[i]->rect = Pim::Rect(78.f, 150.f, 39.f, 38.f);
		powerCogs[i]->position = cogPos[i];
		addChild(powerCogs[i]);
	}

	powerCogs[0]->rotation = -14.f;
	powerCogs[1]->rotation = -7.f;
	powerCogs[2]->rotation = -2.f;
}
void HUDLayer::loadPowerBar()
{
	Pim::Sprite *back = new Pim::Sprite;
	back->useBatchNode(actorSheet);
	back->anchor = Pim::Vec2(1.f, 1.f);
	back->rect = Pim::Rect(301.f, 149.f, 55.f, 114.f);
	back->position = Pim::Vec2(384.f, 216.f);
	addChild(back);

	Pim::Sprite *power = new Pim::Sprite;
	power->useBatchNode(actorSheet);
	power->anchor = Pim::Vec2(0.f, 0.f);
	power->rect = Pim::Rect(0.f, 149.f, 35.f, 59.f);
	power->position = Pim::Vec2(339.f, 119.f);
	addChild(power);
}
void HUDLayer::loadHearts()
{
	for (int i=0; i<3; i++)
	{
		hearts[i] = new Pim::Sprite;
		hearts[i]->useBatchNode(actorSheet);
		hearts[i]->rect = Pim::Rect(32,235,9,9);
		hearts[i]->position = Pim::Vec2(339.f+(13*i), 207.f);
		addChild(hearts[i]);
	}
}

void HUDLayer::update(float dt)
{
	updateLabels(dt);

	weaponCog->rotation += dt * 70.f;
	arrowCog->rotation -= dt * 70.f;

	powerCogs[0]->rotation += dt * 70.f;
	powerCogs[1]->rotation -= dt * 70.f;
	powerCogs[2]->rotation += dt * 70.f;

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
		if (i >= health)
		{
			hearts[i]->rect.x = 42;
		}
	}
}