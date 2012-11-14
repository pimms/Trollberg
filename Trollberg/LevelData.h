#pragma once

#include <string>
#include "Pim.h"

typedef std::vector<std::vector<Pim::Vec2>> Polygons;

struct LevelData
{
	// The texture used by the batchnode.
	std::string batchFile;

	// The color of the sky 
	Pim::Color	color;

	// Triangles used when instantiating the physics 
	Polygons	physics;

	// The backmost parallax layer
	Pim::Rect	parallax0;

	// Parallax layer placed closer to the playfield
	Pim::Rect	parallax1;

	// Parallax layer placed even closer, but still behind the playfield
	Pim::Rect	parallax2;

	// The playfield
	Pim::Rect	playfield;

	// Parallax layer placed in front of the playfield
	Pim::Rect	parallax3;
};