#pragma once
#include "Trollberg.h"

// Forward declarations
class ParallaxLayer;
class GameLayer;

class GameScene : public Pim::Scene
{
public:
	// Returns the Box2D world
	static b2World* getWorld() { return singleton->world; }

	GameScene(int levelNumber);
	~GameScene();

	void loadResources();
	void loadLevelData();
	void loadLayers();

	void update(float dt);

private:
	static GameScene* singleton;

	std::string levelFile;
	LevelData	levelData;

	// The batch object containing the texture used for all
	// the custom layers in the scene
	Pim::SpriteBatchNode	*batch;

	GameLayer				*gameLayer;
	ParallaxLayer			*par0;
	ParallaxLayer			*par1;
	ParallaxLayer			*par2;
	ParallaxLayer			*par3;

	// The Box2D world
	b2World					*world;
};

