#pragma once 
#include "MenuButton.h"

MenuButton::MenuButton(Pim::Sprite *norm, Pim::Sprite *hover, Pim::Sprite *press, Pim::Font *font)
 : Pim::Button(norm, hover, press, NULL)
{
	label = new Pim::Label(font);
	label->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
	label->setTextAlignment(Pim::Label::TEXT_CENTER);
	label->setZOrder(10);
	label->position = Pim::Vec2(0.f, 6.f);
	label->scale = Pim::Vec2(0.2f, 0.2f);
	addChild(label);
}

void MenuButton::setText(std::string txt)
{
 label->setText(txt);
}

void MenuButton::makeHoveredCurrent()
{
	Pim::Button::makeHoveredCurrent();
	label->position = Pim::Vec2(0.f, 6.f);
}
void MenuButton::makePressedCurrent()
{
	Pim::Button::makePressedCurrent();
	label->position = Pim::Vec2(0.f, 5.f);
}