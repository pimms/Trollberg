#pragma once
#include "pimscene.h"

class GameScene : public Pim::Scene
{
public:
	GameScene(std::string lvlfile);

	void loadResources();
	void loadFromFile();


	std::string levelFile;
};

