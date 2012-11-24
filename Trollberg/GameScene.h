#pragma once
#include "Trollberg.h"

// Forward declarations
class ParallaxLayer;
class GameLayer;
class HUDLayer;

class GameScene : public Pim::Scene
{
public:
	// Returns the Box2D world
	static b2World* getWorld() { return singleton->world; }
	static GameScene* getSingleton() { return singleton; }

	GameScene(int levelNumber);
	~GameScene();

	Pim::Layer* pauseLayer();

	void loadResources();
	void loadLevelData();
	void loadLayers();

	void update(float dt);

	float					levelWidth;

private:
	static GameScene*		singleton;

	std::string				levelFile;
	LevelData				levelData;
	int						levelNum;

	// The batch object containing the texture used for all
	// the custom layers in the scene
	Pim::SpriteBatchNode	*batch;

	GameLayer				*gameLayer;
	HUDLayer				*hudLayer;

	ParallaxLayer			*par0;	// Back		(furthest)
	ParallaxLayer			*par1;	// Back
	ParallaxLayer			*par2;	// Back
	ParallaxLayer			*par3;	// Front
	ParallaxLayer			*par4;	// Front	(nearest)

	// The Box2D world
	b2World					*world;
};

