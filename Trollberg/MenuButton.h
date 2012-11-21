#pragma once 
#include "Pim.h"

class MenuButton : public Pim::Button
{
public:
 MenuButton(Pim::Sprite *norm, Pim::Sprite *hover, Pim::Sprite *press, Pim::Font *font);

 void setText(std::string txt);


 void makeHoveredCurrent();
 void makePressedCurrent();

private:
 Pim::Label *label;
};