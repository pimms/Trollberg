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
	int			levelNum;

	// The batch object containing the texture used for all
	// the custom layers in the scene
	Pim::SpriteBatchNode	*batch;

	GameLayer				*gameLayer;
	ParallaxLayer			*par0;	// Back		(furthest)
	ParallaxLayer			*par1;	// Back
	ParallaxLayer			*par2;	// Back
	ParallaxLayer			*par3;	// Front
	ParallaxLayer			*par4;	// Front	(nearest)

	// The Box2D world
	b2World					*world;
};

