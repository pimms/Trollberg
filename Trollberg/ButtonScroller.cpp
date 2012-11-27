#include "ButtonScroller.h"
#include "MenuButton.h"

ButtonScroller::ButtonScroller(Pim::SpriteBatchNode *batch,
				int numButtons, const char **labels, Pim::ButtonCallback *cb)
{
	listenFrame();
	buttonSheet = batch;
	callback = cb;

	scrollingDown	= false;
	scrollingUp		= false;

	// Load the font
	arial40 = new Pim::Font("res\\arial.ttf", 40, false);

	// Load the cogs
	cog1 = new Pim::Sprite;
	cog1->useBatchNode(buttonSheet);
	cog1->rect = Pim::Rect(0.f, 107.f, 58.f, 55.f);
	cog1->position = Pim::Vec2(-105.f, 216.f);
	cog1->rotation = -13.f;
	addChild(cog1);

	cog2 = new Pim::Sprite;
	cog2->useBatchNode(buttonSheet);
	cog2->rect = Pim::Rect(0.f, 107.f, 58.f, 55.f);
	cog2->position = Pim::Vec2(105.f, 216.f);
	cog2->rotation = 12.f;
	addChild(cog2);

	scroller = new Pim::GameNode;
	scroller->position = Pim::Vec2(0.f, 216.f);
	addChild(scroller);

	// Load the rails
	Pim::Sprite *rail = new Pim::Sprite;
	rail->useBatchNode(buttonSheet);
	rail->rect = Pim::Rect(190.f, 0.f, 10.f, 198.f);
	rail->position = Pim::Vec2(-75.f, 99.f);
	scroller->addChild(rail);

	rail = new Pim::Sprite;
	rail->useBatchNode(buttonSheet);
	rail->rect = Pim::Rect(190.f, 0.f, 10.f, 198.f);
	rail->position = Pim::Vec2(75.f, 99.f);
	rail->scale.x = -1.f;
	scroller->addChild(rail);

	// Load the buttons
	//for (int i=numButtons-1; i>=0; i--)
	for (int i=0; i<numButtons; i++)
	{
		MenuButton *btn = createButton(12 + (numButtons-1)*40-(i*40), labels[i]);

		scroller->addChild(btn);
		buttons.push_back(btn);
	}
}
ButtonScroller::~ButtonScroller()
{
	delete arial40;
}

MenuButton* ButtonScroller::createButton(float yPos, const char *label)
{
	Pim::Sprite *normal = new Pim::Sprite;
	normal->useBatchNode(buttonSheet) ; 
	normal->rect = Pim::Rect(0,0,150,35);
	normal->scale = Pim::Vec2(1.0f, 0.7f);

	Pim::Sprite *hovered = new Pim::Sprite;
	hovered->useBatchNode(buttonSheet);
	hovered->rect = Pim::Rect(0,34,150,35);
	hovered->scale = Pim::Vec2(1.0f, 0.7f);

	Pim::Sprite *pressed = new Pim::Sprite;
	pressed->useBatchNode(buttonSheet); 
	pressed->rect = Pim::Rect(0,70,150,35);
	pressed->scale = Pim::Vec2(1.0f, 0.7f);

	MenuButton *menuButton = new MenuButton(normal, hovered, pressed, arial40);
	menuButton->position = Pim::Vec2(192, 108);
	menuButton->setText(label);

	menuButton->setZOrder(1);
	menuButton->setCallback(callback);
	menuButton->position = Pim::Vec2(0.f, yPos);

	return menuButton;
}

void ButtonScroller::update(float dt)
{
	if (scrollingDown)
	{
		cog1->rotation -= dt * 140.f;
		cog2->rotation += dt * 140.f;

		scroller->position.y -= 100.f * dt;
		if (scroller->position.y < 30.f)
		{
			scroller->position.y = 30.f;
			scrollingDown = false;
		}
	}
	else if (scrollingUp)
	{
		cog1->rotation += dt * 140.f;
		cog2->rotation -= dt * 140.f;

		scroller->position.y += 100.f * dt;
		if (scroller->position.y > 216.f)
		{
			scroller->position.y = 216.f;
			scrollingUp = false;
		}
	}
}
void ButtonScroller::scrollDown()
{
	scrollingDown = true;
	scrollingUp = false;
}
void ButtonScroller::scrollUp()
{
	scrollingDown = false;
	scrollingUp = true;
}
bool ButtonScroller::doneScrolling()
{
	return !scrollingDown && !scrollingUp;
}

int ButtonScroller::getButtonID(Pim::Button *btn)
{
	for (unsigned int i=0; i<buttons.size(); i++)
	{
		if (buttons[i] == btn)
		{
			return i;
		}
	}
	return -1;
}