#include "LevelParser.h"


LevelData LevelParser::parseTrollbergLevel(std::string path)
{
	LevelData		level;

	// Parse the physics data
	level.physics = parsePoly(path);

	// Load the file
	TiXmlDocument doc( path.c_str() );
	if (!doc.LoadFile())
	{
		std::string desc = "Could not open PIM-file for parsing:\n";
		desc.append(path);
		TrollbergExit(desc);
	}

	TiXmlElement	*elem = NULL;
	const char		*attr = NULL;;

	// Get the level main texture
	elem = doc.FirstChildElement("batchnode");
	if (elem)
	{
		if (attr = elem->Attribute("tex", (double*)NULL))
			level.batchFile = attr;
		else
			TrollbergExit("No batch tex specified!");

		if (attr = elem->Attribute("color", (double*)NULL))
			level.color = colorFromString(attr);
		else
			std::cout<<"WARNING: NO COLOR SPECIFIED IN THE LEVEL FILE. IT WILL BE WHITE.\n";
	}
	else
		TrollbergExit("Corrupt level file:\nNo level texture specified!");

	// Get the four (possible) parallax layers
	Pim::Rect *rectPtr[4] = {
		&level.parallax0,  &level.parallax1,
		&level.parallax2,  &level.parallax3
	};
	for (int i=0; i<4; i++)
	{
		std::stringstream ss;
		ss <<"parallax" <<i;

		elem = doc.FirstChildElement(ss.str().c_str());
		if (elem)
		{
			if (attr = elem->Attribute("rect", (double*)NULL))
				(*rectPtr[i]) = rectFromString(attr);
		}
	}

	// Get the playfield
	elem = doc.FirstChildElement("playfield");
	if (elem)
	{
		if (attr = elem->Attribute("rect", (double*)NULL))
			level.playfield = rectFromString(attr);
		else
			TrollbergExit("No rect specified for the playfield");
	}
	else
		TrollbergExit("Corrupt level file:\nNo playfield data specified!");

	return level;
}

// ------------------------------------
// ------------------------------------

std::string LevelParser::getResourcePath(std::string path)
{
	TiXmlDocument doc( path.c_str() );
	if (!doc.LoadFile())
	{
		std::string desc = "Could not open file for parsing:\n";
		desc.append(path);
		MessageBoxA(NULL, desc.c_str(), "Error!", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	TiXmlElement *root = NULL;
	root = doc.FirstChildElement("meta");
	
	if (root)
	{
		resPath = root->Attribute("res", (double*)NULL);
	}

	return resPath;
}

void LevelParser::setResourcePath(std::string file, std::string path)
{
	TiXmlDocument doc( file.c_str() );
	if (!doc.LoadFile())
	{
		std::string desc = "Could not open file for writing:\n";
		desc.append(file);
		MessageBoxA(NULL, desc.c_str(), "Error!", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	TiXmlElement *root = NULL;
	root = doc.FirstChildElement("meta");
	if (root)
	{
		root->SetAttribute("res", path.c_str());
		resPath = path;
		doc.SaveFile();
	}
}

Pim::Layer* LevelParser::parse(std::string path)
{
	resPath				= getResourcePath(path);
	batchnode			= NULL;
	Pim::Layer *layer	= NULL;

	TiXmlDocument doc( path.c_str() );
	if (!doc.LoadFile())
	{
		std::string desc = "Could not open file for parsing:\n";
		desc.append(path);
		MessageBoxA(NULL, desc.c_str(), "Error!", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	TiXmlElement *root;
	root = doc.FirstChildElement("layer");
	if (root)
	{
		setLayerAttributes(root, layer);
		parseNode(root, layer);
	}

	return layer;
}

Polygons LevelParser::parsePoly(std::string path)
{
	Polygons ret;

	TiXmlDocument doc( path.c_str() );
	if (!doc.LoadFile())
	{
		std::string desc = "Could not open file for parsing:\n";
		desc.append(path);
		MessageBoxA(NULL, desc.c_str(), "Error!", MB_OK | MB_ICONEXCLAMATION);
		return ret;
	}

	TiXmlElement *root;
	root = doc.FirstChildElement("poly");
	if (root)
	{
		TiXmlElement *cur;
		for (cur = root->FirstChildElement("vert"); cur; cur = cur->NextSiblingElement("vert"))
		{
			const char *attr;
			std::vector<Pim::Vec2> vec;

			attr = cur->Attribute("p1", (double*)NULL);
			if (attr)
				vec.push_back(vecFromString(attr));
			
			attr = cur->Attribute("p2", (double*)NULL);
			if (attr)
				vec.push_back(vecFromString(attr));

			attr = cur->Attribute("p3", (double*)NULL);
			if (attr)
				vec.push_back(vecFromString(attr));

			ret.push_back(vec);
		}
	}

	return ret;
}

// --------------------------------------------------------------


void LevelParser::parseNode(TiXmlElement *elem, Pim::GameNode *node)
{
	TiXmlElement *cur;

	// Nodes
	for (cur = elem->FirstChildElement("node"); cur; cur = cur->NextSiblingElement("node"))
	{
		Pim::GameNode *child = new Pim::GameNode;
		node->addChild(child);

		setNodeAttributes(cur, node);
		parseNode(cur, node);
	}

	// Batch nodes
	for (cur = elem->FirstChildElement("batchnode"); cur; cur = cur->NextSiblingElement("batchnode"))
	{
		if (batchnode != NULL)
		{
			MessageBoxA(
				NULL, 
				"A batch node cannot be child of another batch node.", 
				"Error",
				MB_OK | MB_ICONEXCLAMATION 
			);
			break;
		}

		Pim::SpriteBatchNode *sbn = new Pim::SpriteBatchNode;
		node->addChild(sbn);

		setSpriteAttributes(cur, sbn);
		
		batchnode = sbn;
		parseNode(cur, sbn);
		batchnode = NULL;
	}

	// Sprites
	for (cur = elem->FirstChildElement("sprite"); cur; cur = cur->NextSiblingElement("sprite"))
	{
		Pim::Sprite *sprite = new Pim::Sprite;
		node->addChild(sprite);

		if (batchnode != NULL)
			sprite->useBatchNode(batchnode);

		setSpriteAttributes(cur, sprite);
		parseNode(cur, sprite);
	}

	// Layers
	for (cur = elem->FirstChildElement("layer"); cur; cur->NextSiblingElement("layer"))
	{
		Pim::Layer *layer = new Pim::Layer;
		node->addChild(layer);

		setLayerAttributes(cur, layer);
		parseNode(cur, layer);
	}
}


// --------------------------------------------------------------


void LevelParser::setNodeAttributes(TiXmlElement *elem, Pim::GameNode *node)
{
	// Common
	parsePosition(elem, node);
	parseRotation(elem, node);
	parseIdentifier(elem, node);

	// Custom
}
void LevelParser::setLayerAttributes(TiXmlElement *elem, Pim::Layer *layer)
{
	// Common
	parsePosition(elem, layer);
	parseRotation(elem, layer);
	parseIdentifier(elem, layer);

	// Custom
	//parseLightingSystem(elem, layer);
	parseColor(elem, layer);
	parseImmovable(elem, layer);
	parseScale(elem, layer);
}
void LevelParser::setSpriteAttributes(TiXmlElement *elem, Pim::Sprite *sprite)
{
	// Common
	parsePosition(elem, sprite);
	parseRotation(elem, sprite);
	parseIdentifier(elem, sprite);
	
	// Custom
	parseAnchor(elem, sprite);
	parseImage(elem, sprite);
	parseColor(elem, sprite);
	parseScale(elem, sprite);
}


// --------------------------------------------------------------


Pim::Vec2 LevelParser::vecFromString(const char *str)
{
	const char *x = str;
	const char *y = str;

	for (int i=0; i<strlen(str); i++)
	{
		if (str[i] == ' ')
			y = str+i;
	}

	return Pim::Vec2( 
		(float)atof(x), 
		(float)atof(y) 
	);
}
Pim::Color LevelParser::colorFromString(const char *str)
{
	const char *rgba[4] = {str};

	int idx = 1;
	for (int i=0; i<strlen(str); i++)
	{
		if (str[i] == ' ')
			rgba[idx++] = str+i;
	}

	return Pim::Color(
		(float)atof(rgba[0]),
		(float)atof(rgba[1]),
		(float)atof(rgba[2]),
		(float)atof(rgba[3])
	);
}
Pim::Rect LevelParser::rectFromString(const char *str)
{
	// Rect and color are both just 4D vectors.
	Pim::Color c = colorFromString(str);
	return Pim::Rect(c.r, c.g, c.b, c.a);
}


// --------------------------------------------------------------


void LevelParser::parsePosition(TiXmlElement *elem, Pim::GameNode *node)
{
	const char *attr = elem->Attribute("position", (double*)NULL);

	if (attr != NULL)
	{
		node->position = vecFromString(attr);
	}
}
void LevelParser::parseRotation(TiXmlElement *elem, Pim::GameNode *node)
{
	double attr = 0;
	elem->Attribute("rotation", &attr);

	if (attr != NULL)
	{
		node->rotation = attr;
	}
}
void LevelParser::parseIdentifier(TiXmlElement *elem, Pim::GameNode *node)
{

	const char *attr = elem->Attribute("img", (double*)NULL);
	
	if (attr != NULL)
	{
		node->identifier = attr;
	}
	else
	{
		attr = elem->Attribute("identifier", (double*)NULL);
		
		if (attr != NULL)
		{
			node->identifier = attr;
		}
	}
}

void LevelParser::parseAnchor(TiXmlElement *elem, Pim::Sprite *sprite)
{
	const char *attr = elem->Attribute("anchor", (double*)NULL);
	
	if (attr != NULL)
	{
		sprite->anchor = vecFromString(attr);
	}
}
void LevelParser::parseImage(TiXmlElement *elem, Pim::Sprite *sprite)
{
	const char *attr = elem->Attribute("img", (double*)NULL);

	if (attr != NULL)
	{
		std::string res;

		if (attr[0] != 'C')
		{
			res = resPath;
			res.append(attr);
		}
		else
		{
			res = attr;
		}

		sprite->loadSprite(res);
	}
}
void LevelParser::parseColor(TiXmlElement *elem, Pim::Sprite *sprite)
{
	const char *attr = elem->Attribute("color", (double*)NULL);

	if (attr != NULL)
	{
		sprite->color = colorFromString(attr);
	}
}
void LevelParser::parseScale(TiXmlElement *elem, Pim::Sprite *sprite)
{
	const char *attr = elem->Attribute("scale", (double*)NULL);

	if (attr != NULL)
	{
		sprite->scale = vecFromString(attr);
	}
}
void LevelParser::parseRect(TiXmlElement *elem, Pim::Sprite *sprite)
{
	const char *attr = elem->Attribute("rect", (double*)NULL);

	if (attr != NULL)
	{
		sprite->rect = rectFromString(attr);
	}
}

// TBI // void LevelParser::parseLightingSystem(TiXmlElement *elem, Pim::Layer *layer){}
void LevelParser::parseColor(TiXmlElement *elem, Pim::Layer *layer)
{
	const char *attr = elem->Attribute("color", (double*)NULL);

	if (attr != NULL)
	{
		layer->color = colorFromString(attr);
	}
}
void LevelParser::parseImmovable(TiXmlElement *elem, Pim::Layer *layer)
{
	double attr = 0;
	elem->Attribute("immovable", &attr);

	if (attr != 0)
	{
		layer->immovable = (bool)attr;
	}
}
void LevelParser::parseScale(TiXmlElement *elem, Pim::Layer *layer)
{
	const char *attr = elem->Attribute("scale", (double*)NULL);

	if (attr != NULL)
	{
		layer->scale = vecFromString(attr);
	}
}