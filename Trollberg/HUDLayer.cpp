#include "HUDLayer.h"
#include "Troll.h"

HUDLayer::HUDLayer()
{
	immovable = true;
	Troll::setHUDLayer(this);
}
