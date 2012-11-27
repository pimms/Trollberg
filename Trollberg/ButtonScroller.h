#pragma once
#include "Pim.h"

// Forward declarations
class MenuButton;

// ButtonScroller defines the object which will simulate two cogwheels scrolling down a
// set of buttons.
class ButtonScroller : public Pim::Sprite
{
public:
	ButtonScroller(Pim::SpriteBatchNode *buttonSheet,
			int numButtons, const char **labels, Pim::ButtonCallback *cb);
	~ButtonScroller();

	void update(float dt);
	void scrollDown();
	void scrollUp();
	bool doneScrolling();

	// If the button is a child of ButtonScroller, it's index in the 'buttons'array is
	// returned.
	int getButtonID(Pim::Button *btn);

private:

	MenuButton* createButton(float yPos, const char *label);

	Pim::Font				*arial40;
	Pim::SpriteBatchNode	*buttonSheet;
	Pim::ButtonCallback		*callback;

	bool					scrollingDown;
	bool					scrollingUp;

	// The cogwheels
	Pim::Sprite				*cog1;
	Pim::Sprite				*cog2;

	// The object being moved vertically
	Pim::GameNode			*scroller;

	// The buttons
	std::vector<MenuButton*> buttons;
};

