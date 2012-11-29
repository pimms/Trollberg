#pragma once
#include "Pim.h"
#define HS_MAXLABELS 6

class HighscoreLayer : public Pim::Layer
{
public:
	HighscoreLayer(bool onlyShow);
	~HighscoreLayer(void);

	void loadResources();
	void update(float dt);

	Pim::Font* arial40;

	Pim::Label* theLables[HS_MAXLABELS];

	//her lagres fildataen, fuck det er frodekode
	char lastFileData[200][2][40];
	int numData;
	bool scoreStored;

	bool onlyShow;

	std::string yourName;
	bool nameEntered;

	Pim::Label		*tittleLabel;
	Pim::Label		*youText;
	Pim::Label		*youName;

	void updateText(int xPos, int yPos, int index, std::string text);

	void mouseEvent(Pim::MouseEvent &evt);

	void updateScoreList();
	void addMyScore();

	void keyEvent(Pim::KeyEvent&);


};

