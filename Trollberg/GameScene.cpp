#include "GameScene.h"
#include "LevelParser.h"
#include "ParallaxLayer.h"
#include "GameLayer.h"

GameScene::GameScene(int levelNumber)
{
	std::stringstream ss;
	ss <<"tbl" <<levelNumber <<".pim";
	levelFile = ss.str();

	batch = NULL;
}
GameScene::~GameScene()
{
	// As the GameScene does not inherit from the GameNode-class, all 
	// memory management has do be performed manually.
	if (batch) delete batch;
}

void GameScene::loadResources()
{
	loadLevelData();
}
void GameScene::loadLevelData()
{
	LevelParser parser;
	levelData = parser.parseTrollbergLevel(levelFile);
	color = levelData.color;

	batch = new Pim::SpriteBatchNode(levelData.batchFile);

	loadLayers();
}
void GameScene::loadLayers()
{
	// Game layer
	gameLayer = new GameLayer;
	addLayer(gameLayer);
	gameLayer->setSpriteInformation(batch, levelData.playfield);

	// Parallax layer #0
	if (levelData.parallax0.width != 0)
	{
		par0 = new ParallaxLayer(gameLayer, 0.2f);
		addLayer(par0);
		par0->setZOrder(-5);
		par0->setSpriteInformation(batch, levelData.parallax0);
	}

	// Parallax layer #1
	if (levelData.parallax1.width != 0)
	{
		par1 = new ParallaxLayer(gameLayer, 0.35f);
		addLayer(par1);
		par1->setZOrder(-4);
		par1->setSpriteInformation(batch, levelData.parallax1);
	}

	// Parallax layer #2
	if (levelData.parallax2.width != 0)
	{
		par2 = new ParallaxLayer(gameLayer, 0.55f);
		addLayer(par2);
		par2->setZOrder(-3);
		par2->setSpriteInformation(batch, levelData.parallax2);
	}

	// Parallax layer #3
	if (levelData.parallax3.width != 0)
	{
		par3 = new ParallaxLayer(gameLayer, 0.35f);
		addLayer(par3);
		par3->setZOrder(-2);
		par3->setSpriteInformation(batch, levelData.parallax3);
	}
}

void GameScene::update(float dt)
{

}