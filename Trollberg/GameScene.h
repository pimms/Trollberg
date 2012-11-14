#pragma once
#include "Trollberg.h"

// Forward declarations
class ParallaxLayer;
class GameLayer;

class GameScene : public Pim::Scene
{
public:
	GameScene(int levelNumber);
	~GameScene();

	void loadResources();
	void loadLevelData();
	void loadLayers();

	void update(float dt);

private:
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
};

