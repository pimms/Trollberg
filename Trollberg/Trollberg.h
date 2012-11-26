#pragma once


#include <sstream>
#include <math.h>

#include "Pim.h"
#include "tinyxml.h"
#include "Box2D\Box2D.h"

#include "LevelData.h"

typedef std::vector<std::vector<Pim::Vec2>> Polygons;

// Box2D meters to pixel-ratio. There are PTMR pixels in one B2-meter.
// PTMR = Pixels To Meters Radius
#define PTMR 10

// Degrees to radians factor. Multiply a degree value with DEGTORAD to obtain radians.
#define DEGTORAD (M_PI/180.f)

// Radians to degrees factor. Multiply a radian value with RADTODEG to obtain degrees.
#define RADTODEG (180.f/M_PI)


// Filtering used for collisions. When creating an actor, is must be provided with certain
// values to determine what it is (collision group) and what is will collide with (mask).
// If a troll were to collide with the player and the ground, but not other trolls, it's
// body should be created as such:
// createCircularBody(radius, TROLLS, GROUND | PLAYER);
#define GROUND			1
#define	PLAYER			2
#define SENSOR			8
#define LVLEDGE			16
#define TROLLS_SL		(32  )
#define TROLLS_TL		(64	 )
#define TROLLS_C		(128 )
#define TROLLS			(TROLLS_SL|TROLLS_TL|TROLLS_C)

#define SCREENWIDTH		384
#define SCREENHEIGHT	216



// Pim::Vec2 to b2Vec2 conversion. PTM-Ratio is taken into account.
inline b2Vec2 toB2(const Pim::Vec2 &v)
{
	return b2Vec2(v.x / PTMR, v.y / PTMR);
}

// b2Vec2 to Pim::Vec2 conversion. PTM-Ratio is taken into account.
inline Pim::Vec2 toPim(const b2Vec2 &v)
{
	return Pim::Vec2(v.x * PTMR, v.y * PTMR);
}

// Takes any value, and makes sure it's within the 0-360 range
inline float normDeg(float deg)
{
	while (deg < 0.f)
	{
		deg += 360.f;
	}
	while (deg > 360.f)
	{
		deg -= 360.f;
	}

	return deg;
}

// Something went horribly wrong. Display an error message and exit the game completely.
void TrollbergExit(std::string s);