#include "Trollberg.h"

#include "Weapon.h"
#include "GameLayer.h"
#include "TrollControl.h"
#include "Player.h"

#include "Troll.h"
#include "Slinker.h"
#include "Tumbler.h"
#include "Colossus.h"

#include "GameScene.h"

TrollControl* TrollControl::singleton = NULL;

TrollControl::TrollControl(Pim::SpriteBatchNode *batch, GameLayer *l, 
						   Player *pl, float lvlWidth, int lvl)
{
	singleton = this;

	actorSheet	= batch;
	player		= pl;
	levelWidth	= lvlWidth;
	levelNum	= lvl;
	layer		= l;

	this->pl = pl;

	trollSpawnTimer = 0.f;
	trollsToSpawn = 100000;
	trollsOnScreen = 0;
	maxTrollsOnScreen = 10;
	trollTimeBetweenSpawn = 0.05f;

	//for (int i=0; i<5; i++)
	//{
	//	Tumbler *tl = new Tumbler(pl, actorSheet, Pim::Vec2(500+50*i, 150.f));
	//	layer->addChild(tl);
	//}

	printf("haha");

	// pos 0 i emthy;
	//in first arr pos: 1 = lvl 1, 2 = lvl 2, 3 = lvl 3
	//in sec arr pos:  1 = stinker, 2 = tumbler, 3 = colossus

	//lvl 1
	trollSpawnChance[1][1] = 60;
	trollSpawnChance[1][2] = 35;
	trollSpawnChance[1][3] = 5;

	//lvl 2
	trollSpawnChance[2][1] = 40;
	trollSpawnChance[2][2] = 40;
	trollSpawnChance[2][3] = 20;

	//lvl3
	trollSpawnChance[3][1] = 20;
	trollSpawnChance[3][2] = 40;
	trollSpawnChance[3][3] = 40;

	//extra delay when spawning one type of troll:
	//lvl 1
	trollSpawnDelay[1][1] = 0.f;
	trollSpawnDelay[1][2] = 1.f;
	trollSpawnDelay[1][3] = 2.f;

	//lvl 2
	trollSpawnDelay[2][1] = 0.f;
	trollSpawnDelay[2][2] = 1.f;
	trollSpawnDelay[2][3] = 2.f;

	//lvl3
	trollSpawnDelay[3][1] = 0.f;
	trollSpawnDelay[3][2] = 0.f;
	trollSpawnDelay[3][3] = 1.f;
}
TrollControl::~TrollControl(void)
{
	singleton = NULL;
}

void TrollControl::update(float dt){

	if(trollSpawnTimer > trollTimeBetweenSpawn && trollsOnScreen < maxTrollsOnScreen)
	{

		if(trollsToSpawn > 0)
		{
			spawnTroll();
		}
		else
		{
			printf("Du er ferdig");
			trollSpawnTimer = 0.f;
		}

		

	}
	else
	{
		trollSpawnTimer += dt;
	}
	

}

void TrollControl::spawnTroll(){

	//do{
		//randum nomber, -1 or 1:
		ranDirr = ((rand() % 2) * 2) -1;
		//get spawn x ca 60 pc offscreen to the player at a random direction:
		spawnX = (pl->position.x) + ((SCREENWIDTH + 50 + rand() % 40) * ranDirr);
	//}
	//while(spawnX < 0 || spawnX > GameScene::getSingleton()->levelWidth);

	//std::cout << ranDirr << "|" << ((SCREENWIDTH + 50 + rand() % 40)) << "\n";
	//std::cout <<  pl->position.x << "|"  << (pl->position.x - SCREENWIDTH) << "|" << (pl->position.x - SCREENWIDTH - 20 - rand()%15) << std::endl;

	//int randen = (levelNum == 1) ? 30 : (levelNum == 2) ? 40 : (levelNum == 3) ? 60 : 0;

	int randNum = rand() % 101;
	std::cout << randNum << "<=" << trollSpawnChance[levelNum][1] << "\n";
	if(randNum <= trollSpawnChance[levelNum][1])
	{

		Slinker *sl = new Slinker(pl, actorSheet, Pim::Vec2(spawnX, 150.f));
		layer->addChild(sl);
		trollSpawnTimer = -trollSpawnDelay[levelNum][1];

	}
	else if(randNum <= trollSpawnChance[levelNum][1]+trollSpawnChance[levelNum][2])
	{

		Tumbler *tl = new Tumbler(pl, actorSheet, Pim::Vec2(spawnX, 150.f));
		layer->addChild(tl);
		trollSpawnTimer = -trollSpawnDelay[levelNum][2];

	}
	else if(randNum <= trollSpawnChance[levelNum][1]+trollSpawnChance[levelNum][2]+trollSpawnChance[levelNum][3])
	{
		Colossus *c = new Colossus(pl, actorSheet, Pim::Vec2(spawnX, 150.f));
		layer->addChild(c);
		c->createLight();
		trollSpawnTimer = -trollSpawnDelay[levelNum][3];
	}
	else
	{
		std::cout << "ERROR ON  SPAWN LOG?";
	}

	trollsToSpawn --;
	trollsOnScreen ++;

}

void TrollControl::trollKilled(){

	trollsOnScreen --;

}
