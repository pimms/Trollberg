#pragma once

#include <sstream>

#include "Pim.h"
#include "tinyxml.h"
#include "Box2D\Box2D.h"

#include "LevelData.h"

typedef std::vector<std::vector<Pim::Vec2>> Polygons;



// Something went horribly wrong. Display an error message and exit the game completely.
void TrollbergExit(std::string s);