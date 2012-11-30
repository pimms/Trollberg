#include "HUDLayer.h"
#include "Troll.h"
#include "GameLayer.h"
#include "HighscoreLayer.h"


// The lifetime of the floating dmgLabels
#define FL_LIFETIME 0.5f


HUDLayer* HUDLayer::singleton = NULL;
Pim::Font* HUDLayer::arial40 = NULL;


HUDLayer* HUDLayer::getSingleton()
{
	return singleton;
}

void HUDLayer::createFont()
{
	if (!arial40)
	{
		arial40 = new Pim::Font("res\\arial.ttf", 40, false);
	}
}
Pim::Font* HUDLayer::getFont()
{
	return arial40;
}
void HUDLayer::destroyFont()
{
	if (arial40)
	{
		delete arial40;
	}
}

HUDLayer::HUDLayer()
{
	createFont();

	singleton		= this;
	immovable		= true;

	playerDead		= false;
	theHighscoreLayer = NULL;

	isFading		= true;
	fadeTimer		= 0.f;

	score			= 0;
	scoreLabel		= NULL;
	displayScore	= 0;
	scoreToDisplay	= 0;

	curWeapon		= 0;
	weaponRotDest	= 60.f;
	weaponRotDir	= 1;
	damageFadeTimer = 0.f;

	drawPower		= true;
	timer			= 0.f;
}
HUDLayer::~HUDLayer()
{
	destroyFont();

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

	// Load the red border outline
	damageIndicator = new Pim::Sprite;
	damageIndicator->rect = Pim::Rect(130, 4, 1, 1);
	damageIndicator->scale = Pim::Vec2(384.f, 216.f);
	damageIndicator->useBatchNode(actorSheet);
	damageIndicator->anchor = Pim::Vec2(0.f, 0.f);
	damageIndicator->color.a = 0.f;
	addChild(damageIndicator);

	// Load the score label
	scoreLabel = new Pim::Label(arial40);
	scoreLabel->color = Pim::Color(1.f, 0.8f, 0.f, 1.f);
	scoreLabel->position = Pim::Vec2(83.f, 192.f);
	scoreLabel->setTextAlignment(Pim::Label::TEXT_RIGHT);
	scoreLabel->scale *= 0.2f;
	scoreLabel->setText("0");
	addChild(scoreLabel);

	// Load the FPS label
	FPSLabel = new Pim::Label(arial40);
	FPSLabel->color = Pim::Color(1.f, 1.f, 0.f, 1.f);
	FPSLabel->setTextAlignment(Pim::Label::TEXT_RIGHT);
	FPSLabel->position = Pim::Vec2(384.f, 20.f);
	FPSLabel->scale = Pim::Vec2(0.2f, 0.2f);
	addChild(FPSLabel);
}
void HUDLayer::loadHelperSprites()
{
	/*
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
	*/

	// The score background
	Pim::Sprite *scb = new Pim::Sprite;
	scb->useBatchNode(actorSheet);
	scb->anchor = Pim::Vec2(0.5f, 1.f);
	scb->rect = Pim::Rect(0.f, 257.f, 54.f, 33.f);
	scb->position = Pim::Vec2(60.f, 216.f);
	addChild(scb);
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

void HUDLayer::draw()
{
	Pim::Layer::draw();	// Superclass draw

	if (drawPower)
	{
		glPushMatrix();	
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);

		glColor4f(1.f, 0.f, 0.f, 1.f);

		Pim::Vec2 scale = Pim::GameControl::getSingleton()->windowScale();
		glScalef(scale.x, scale.y, 1.f);

		glBegin(GL_TRIANGLE_FAN);
			float yv = 124.f + 47.f * GameLayer::playerFuel();
			glVertex2f(343.f, 124.f);
		
			for (int i=0; i<=11; i++)
			{
				glVertex2f(343.f+i, yv + sinf((timer+i)*5.f)*0.4f + 0.4f);
			}

			glVertex2f(354.f, 124.f);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

void HUDLayer::update(float dt)
{
	updateLabels(dt);
	updateWeaponCog(dt);

	timer += dt;
	
	damageFadeTimer -= dt;
	damageIndicator->color.a = damageFadeTimer;

	if(playerDead)
	{
		showHighscore();
	}


	powerCogs[0]->rotation += dt * 70.f;
	powerCogs[1]->rotation -= dt * 70.f;
	powerCogs[2]->rotation += dt * 70.f;

	FPSLabel->setTextWithFormat("FPS: %0.0f\n", 1.f/dt);

	// Update the score label
	float scoreAppend = dt * (10.f * scoreToDisplay);
	if (scoreAppend > scoreToDisplay)
	{
		scoreAppend = scoreToDisplay;
	}

	scoreToDisplay -=  scoreAppend;
	displayScore += scoreAppend;
	scoreLabel->setTextWithFormat("%0.0f\n", displayScore);


	if (isFading)	// Only occurs for the first 0.5 seconds of the game
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
}
void HUDLayer::updateWeaponCog(float dt)
{
	if (weaponRotDir > 0)
	{
		weaponCog->rotation += dt * 250.f;

		weaponCog->rotation = normDeg(weaponCog->rotation);

		if (weaponCog->rotation >= weaponRotDest && weaponCog->rotation-weaponRotDest < 180.f)
		{
			weaponCog->rotation = weaponRotDest;
			weaponRotDir = 0;
		}
	}
	else if (weaponRotDir < 0)
	{
		weaponCog->rotation -= dt * 250.f;

		weaponCog->rotation = normDeg(weaponCog->rotation);

		if (weaponCog->rotation <= weaponRotDest && weaponRotDest-weaponCog->rotation < 180.f)
		{
			weaponCog->rotation = weaponRotDest;
			weaponRotDir = 0;
		}
	}

	arrowCog->rotation = normDeg(-weaponCog->rotation);
}

void HUDLayer::updateLabels(float dt)
{
	for (unsigned int i=0; i<dmgLabels.size(); i++)
	{
		dmgLabels[i]->lifetime		+= dt;
		dmgLabels[i]->color.a		-= dt / FL_LIFETIME;

		// f(x) = 2*pow((x-0.1),3) + 0.2*sin((x+0.1)*6)
		// http://www.abhortsoft.hu/functionvisualizer/functionvisualizer.html
		float *x = &dmgLabels[i]->lifetime;
		float fx = 2.f*pow((*x-0.1f),3.f) + 0.2f*sinf((*x+0.1f)*6.f);
		dmgLabels[i]->scale = Pim::Vec2(1.f, 1.f) * fx;

		dmgLabels[i]->position.x	 = dmgLabels[i]->initialX + GameLayer::getSingleton()->position.x;
		dmgLabels[i]->position.y	+= dt * 60.f;

		if (dmgLabels[i]->lifetime >= FL_LIFETIME)
		{
			removeChild(dmgLabels[i], true);
			dmgLabels.erase(dmgLabels.begin() + i);
		}
	}

	for (unsigned int i=0; i<scoreLabels.size(); i++)
	{
		scoreLabels[i]->lifetime += dt;

		// Update the position
		float lifeFac = scoreLabels[i]->lifetime / scoreLabels[i]->moveDuration;
		scoreLabels[i]->position = scoreLabels[i]->initialPos + scoreLabels[i]->moveVector*lifeFac;

		// Update the scale
		// f(x) = 2*pow(((x/2)-0.1),3) + 0.4*sin(((x/2)+0.1)*6)+1
		// http://www.abhortsoft.hu/functionvisualizer/functionvisualizer.html
		float *x = &lifeFac;
		float fx = 2.f*pow(((*x/2.f)-0.1f),3.f) + 0.4f*sinf(((*x/2.f)+0.1f)*6.f)+0.1f;
		scoreLabels[i]->scale = Pim::Vec2(0.4f, 0.4f) * fx;

		if (scoreLabels[i]->lifetime > scoreLabels[i]->moveDuration)
		{
			scoreToDisplay += scoreLabels[i]->score;

			removeChild(scoreLabels[i], true);
			scoreLabels.erase(scoreLabels.begin() + i);
		}
	}
}
void HUDLayer::addDamageLabel(Troll *t, int damage)
{
	DamageLabel *lab = new DamageLabel(arial40);
	lab->setTextWithFormat("%d\n", damage);
	lab->scale = Pim::Vec2(0.1f, 0.1f);
	lab->color = Pim::Color(1.f, 1.f, 0.f, 1.f);
	lab->position = t->position + t->getParentLayer()->position + Pim::Vec2(0.f, 15.f);
	lab->initialX = t->position.x;

	addChild(lab);
	dmgLabels.push_back(lab);
}
void HUDLayer::addScoreLabel(Troll *t, int s)
{
	// Increase the score, but don't show it until the label reaches the score label
	score += s;

	ScoreLabel *lab = new ScoreLabel(arial40);
	lab->setTextWithFormat("%d\n", s);
	lab->scale = Pim::Vec2(0.1f, 0.1f);
	lab->color = Pim::Color(1.f, 0.8f, 0.f, 1.f);
	lab->position = t->position + t->getParentLayer()->position;
	lab->initialPos = lab->position;
	lab->score = s;

	lab->moveVector = scoreLabel->position-lab->position;
	lab->moveDuration = log(lab->moveVector.length()) / 10.f;
	
	addChild(lab);
	scoreLabels.push_back(lab);
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
void HUDLayer::playerTookDamage()
{
	damageFadeTimer = 0.3f;
}
void HUDLayer::setSelectedWeapon(int wep)
{
	if (abs(wep-curWeapon) == 2)
	{
		weaponRotDir = curWeapon - wep;
		weaponRotDest = 60 + 120*wep;
	}
	else
	{
		weaponRotDir = wep - curWeapon;
		weaponRotDest = 60 + 120*wep;
	}

	curWeapon = wep;
}

void HUDLayer::showHighscore()
{
	drawPower = false;

	if(theHighscoreLayer == NULL)
	{
		theHighscoreLayer = new HighscoreLayer(false);
		addChild(theHighscoreLayer);
		theHighscoreLayer->loadResources();
		theHighscoreLayer->position = Pim::Vec2(SCREENWIDTH/2, SCREENHEIGHT/2);
	}
}