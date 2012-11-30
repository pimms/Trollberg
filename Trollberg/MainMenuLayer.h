#pragma once 
#include "Pim.h"

class HighscoreLayer;

// Defines
#define NUMMENYBUTTONS 4
#define MENYSPEED 1

// Enumeration defining what will happen once a ButtonScroller has reached the top.
enum ScrollCompleteID
{
	NONE,			// jack diddly squat
	SHOW_MAIN,		// Show the main-scroller
	SHOW_PLAY,		// Show the play-scroller
	PLAY,			// Play the game
	EXIT,			// Exit the game
};

// Forward declaratins
class ButtonScroller;

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback
{
public:
	MainMenuLayer();
	~MainMenuLayer();

	void loadResources();
	void loadButtons();
	void loadSprites();
	void loadParallax();
	void loadRain();

	void buttonPressed(Pim::Button*);

	void update(float dt);
	void updateRain(float dt);
	void updateScroll(float dt);

	void scrollCompleted();

	void keyEvent(Pim::KeyEvent&);

private:
	Pim::SpriteBatchNode	*buttonSheet;
	Pim::SpriteBatchNode	*scrollSheet;

	Pim::Sound				*music;

	ButtonScroller			*activeScroller;
	bool					buttonsScrolling;
	ScrollCompleteID		scrollID;

	ButtonScroller			*mainScroller;
	ButtonScroller			*playScroller;

	// The light we will follow through the forest
	Pim::GameNode			*light;

	// The rain droplets
	std::vector<Pim::Sprite*>	rain;

	// The destination we're scrolling to
	bool					isScrolling;
	float					scrollDest;

	// How many times have we scrolled? 
	// 1 = show menu
	// 2 = go to game
	int						numScrolls;

	// Button dropdown
	bool					startGame;

	int						startLVL;

	HighscoreLayer *theHighscoreLayer;
};