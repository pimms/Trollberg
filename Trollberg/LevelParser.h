#pragma once

#include "Trollberg.h"

/*
	This class is taken from the PimEdit-application, and thus contains a lot of
	member functions never used by the game. 

	"LevelData parseTrollbergLevel(...)" is the only function called on a parser-object.
*/

class LevelParser
{
public:

	// Returns a LevelData-object
	LevelData parseTrollbergLevel(std::string path);

	// Returns the resource path from the meta data in the provided file
	std::string getResourcePath(std::string path);

	// Changes the resource path in the meta data in the provided file
	void setResourcePath(std::string file, std::string resPath);

	// Parse a level file, return a layer
	Pim::Layer* parse(std::string path);

	// Parse a level file, return a polygon-array
	Polygons parsePoly(std::string path);

	std::string resPath;
	Pim::SpriteBatchNode *batchnode;

private:

	// Handles children of the node
	void parseNode(TiXmlElement *elem, Pim::GameNode *node);

	// Parses attributes and sets them accordingly on the passed node
	void setNodeAttributes(TiXmlElement *elem, Pim::GameNode *node);

	// Parses attributes and sets them accordingly on the passed layer
	void setLayerAttributes(TiXmlElement *elem, Pim::Layer *layer);

	// Parses attributes and sets them accordingly on the passed sprite
	void setSpriteAttributes(TiXmlElement *elem, Pim::Sprite *sprite);

	
	//////////////////////////  NOTICE  //////////////////////////////
	//		   The following methods reads a string input,          //
	//     and returns a Pim::Vec2 or a Pim::Color structure.	    //
	//////////////////////////////////////////////////////////////////

	Pim::Vec2 vecFromString(const char *str);
	Pim::Color colorFromString(const char *str);
	Pim::Rect rectFromString(const char *str);
	
	
	//////////////////////////  NOTICE  //////////////////////////////
	//		The following methods all parse the data and            //
	//	 sets an attribute accordingly. The attribute should        //
	//	     be easily identifiable by the method names.            //
	//////////////////////////////////////////////////////////////////
	
	/*
		COMMON ATTRIBUTES
	*/
	void parsePosition(TiXmlElement *elem, Pim::GameNode *node);
	void parseRotation(TiXmlElement *elem, Pim::GameNode *node);
	void parseIdentifier(TiXmlElement *elem, Pim::GameNode *node);

	/*
		SPRITE ATTRIBUTES
	*/
	void parseAnchor(TiXmlElement *elem, Pim::Sprite *sprite);
	void parseImage(TiXmlElement *elem, Pim::Sprite *sprite);
	void parseColor(TiXmlElement *elem, Pim::Sprite *sprite);
	void parseScale(TiXmlElement *elem, Pim::Sprite *sprite);
	void parseRect(TiXmlElement *elem, Pim::Sprite *sprite);

	/*
		LAYER ATTRIBUTES
	*/
	//void parseLightingSystem(TiXmlElement *elem, Pim::Layer *layer);
	void parseColor(TiXmlElement *elem, Pim::Layer *layer);
	void parseImmovable(TiXmlElement *elem, Pim::Layer *layer);
	void parseScale(TiXmlElement *elem, Pim::Layer *layer);
};

