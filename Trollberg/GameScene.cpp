#include "GameScene.h"
#include "LevelParser.h"
#include "PauseLayer.h"
#include "ParallaxLayer.h"
#include "GameLayer.h"
#include "HUDLayer.h"
#include "Entity.h"
#include "Troll.h"

GameScene* GameScene::singleton = NULL;

GameScene::GameScene(int levelNumber)
{
	singleton = this;
	levelNum  = levelNumber;

	std::stringstream ss;
	ss <<"tbl" <<levelNum <<".pim";
	levelFile = ss.str();

	batch = NULL;
	world = NULL;
}
GameScene::~GameScene()
{
	singleton = NULL;

	// As the GameScene does not inherit from the GameNode-class, all 
	// memory management regarding batch nodes has do be performed manually.
	if (batch) delete batch;

	// Remove all bodies. Unless a body is destroyed prematurely to the 
	// game's finish, it is cleaned up automatically.
	if (world)
	{
		while (world->GetJointCount())
			world->DestroyJoint( world->GetJointList() );

		while (world->GetBodyCount())
			world->DestroyBody( world->GetBodyList() );

		delete world;
	}
}

Pim::Layer* GameScene::pauseLayer()
{
	PauseLayer *pl = new PauseLayer;
	pl->setZOrder(1000);
	return pl;
}

void GameScene::loadResources()
{
	// Create the Box2D world
	world = new b2World(b2Vec2(0, -30));

	// Load stuff
	loadLevelData();
	loadLayers();

	Pim::GameControl::getSingleton()->limitFrame(2);
}
void GameScene::loadLevelData()
{
	LevelParser parser;
	levelData = parser.parseTrollbergLevel(levelFile);
	color = levelData.color;

	batch = new Pim::SpriteBatchNode(levelData.batchFile);
}
void GameScene::loadLayers()
{
	// Game layer
	gameLayer = new GameLayer(levelNum);
	addLayer(gameLayer);
	gameLayer->setSpriteInformation(batch, levelData.playfield);
	gameLayer->createGroundBody(levelData.physics);

	// HUD layer
	hudLayer = new HUDLayer;
	hudLayer->setZOrder(100);
	addLayer(hudLayer);

	// Parallax layer #0
	if (levelData.parallax0.width != 0)
	{
		par0 = new ParallaxLayer(gameLayer, 0.3f);
		addLayer(par0);
		par0->setZOrder(-5);
		par0->setSpriteInformation(batch, levelData.parallax0);
	}

	// Parallax layer #1
	if (levelData.parallax1.width != 0)
	{
		par1 = new ParallaxLayer(gameLayer, 0.5f);
		addLayer(par1);
		par1->setZOrder(-4);
		par1->setSpriteInformation(batch, levelData.parallax1);
	}

	// Parallax layer #2
	if (levelData.parallax2.width != 0)
	{
		par2 = new ParallaxLayer(gameLayer, 0.7f);
		addLayer(par2);
		par2->setZOrder(-3);
		par2->setSpriteInformation(batch, levelData.parallax2);
	}

	// Parallax layer #3
	if (levelData.parallax3.width != 0)
	{
		par3 = new ParallaxLayer(gameLayer, 1.35f);
		addLayer(par3);
		par3->setZOrder(2);
		par3->setSpriteInformation(batch, levelData.parallax3);
	}

	// Parallax layer #4
	if (levelData.parallax4.width != 0)
	{
		par4 = new ParallaxLayer(gameLayer, 1.8f);
		addLayer(par4);
		par4->setZOrder(3);
		par4->setSpriteInformation(batch, levelData.parallax4);
	}
}

void GameScene::update(float dt)
{
	if (world)
	{
		world->Step(1.f/60.f, 10, 10);

		// Iterate over all the bodies, update position and rotation
		for (auto it=world->GetBodyList(); it; it=it->GetNext())
		{
			void *ud = it->GetUserData();
			if (ud)
			{
				Entity *ent = static_cast<Entity*>(ud);

				if (ent->body == it)
				{
					ent->position = toPim( ent->body->GetPosition() );
					ent->rotation = ent->body->GetAngle() * RADTODEG;
				}
			}
		}
	}
}