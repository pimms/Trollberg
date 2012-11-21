#include "GameLayer.h"
#include "GameScene.h"
#include "Player.h"
#include "TrollControl.h"
#include "Troll.h"

GameLayer::GameLayer(int lvl)
{
	levelNum		= lvl;
	world			= GameScene::getWorld();
	groundBody		= NULL;
	trollControl	= NULL;
}
GameLayer::~GameLayer()
{
	if (trollControl)
		delete trollControl;

	Troll::destroyFont();
}

void GameLayer::draw()
{
	Pim::Layer::draw();

	return;	// Comment the return to enable physics debug-drawing

	glPushMatrix();
	glLoadIdentity();

	Pim::Vec2 fac = Pim::GameControl::getSingleton()->coordinateFactor();
	glTranslatef(position.x / fac.x, position.y / fac.y, 0.f);

	fac = Pim::GameControl::getSingleton()->windowScale();
	glScalef(scale.x * fac.x * PTMR, scale.y * fac.y * PTMR, 1.f);

	glColor4ub(255,0,0,80);

	glDisable(GL_TEXTURE_2D);

	for (auto b=world->GetBodyList(); b; b=b->GetNext())
	{
		for (auto f=b->GetFixtureList(); f; f=f->GetNext())
		{
			b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(f->GetShape());
			if (shape)
			{
				glPushMatrix();
				glTranslatef(b->GetPosition().x, b->GetPosition().y, 0.f);
				glRotatef(b->GetAngle()*RADTODEG, 0.f, 0.f, 1.f);

				glBegin(GL_POLYGON);
				for (int i=0; i<shape->GetVertexCount(); i++)
				{
					glVertex2f(shape->GetVertex(i).x, shape->GetVertex(i).y);
				}
				glEnd();

				glPopMatrix();
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
void GameLayer::loadResources()
{
	listenFrame();
	listenKeys();
	Troll::createFont();
	loadLightingSystem();

	actorSheet = new Pim::SpriteBatchNode("res\\player.png");
	addChild(actorSheet);

	player = new Player(actorSheet, Pim::Vec2(1070.f, 60.f));
	addChild(player);
	player->createLight();

	trollControl = new TrollControl(actorSheet, this, player, levelWidth, levelNum);
}
void GameLayer::loadLightingSystem()
{
	// Create the lighting system
	createLightingSystem(Pim::Vec2(1920.f, 1080.f));
	setLightingUnlitColor(Pim::Color(0.f, 0.f, 0.f, 0.85f));
	setCastShadows(false);

	// The Lighting Rifle bullet tex
	Pim::SmoothLightDef *ld = new Pim::SmoothLightDef;
	ld->radius = 150.f + rand() % 150;
	ld->innerColor.a = 0.7f;
	preloadLightTexture(ld, "LRBullet");
}

void GameLayer::setSpriteInformation(Pim::SpriteBatchNode *b, Pim::Rect r)
{
	levelWidth = r.width;

	ground = new Pim::Sprite;
	ground->useBatchNode(b);
	ground->anchor = Pim::Vec2(0.f, 0.f);
	ground->rect = r;
	addChild(ground);
}
void GameLayer::createGroundBody(Polygons &poly)
{
	for (int i=0; i<poly.size(); i++)
	{
		b2Vec2 verts[3] = {
			toB2(poly[i][0]/3.333f),
			toB2(poly[i][1]/3.333f),
			toB2(poly[i][2]/3.333f),
		};

		// Ensure that all polygons are wound CCW
		for (int i=0; i<3; i++)
		{
			int i2 = (i+1 < 3) ? (i+1) : (0);
			b2Vec2 edge = verts[i2] - verts[i];

			for (int j=0; j<3; j++)
			{
				if (j == i || j == i2)
					continue;

				b2Vec2 r = verts[j] - verts[i];
				if (b2Cross(edge, r) <= 0.f)
				{
					b2Vec2 tmp = verts[j];
					verts[j] = verts[i];
					verts[i] = tmp;
				}
			}
		}

		b2BodyDef bd;
		bd.type = b2_staticBody;

		b2PolygonShape shape;
		shape.Set(verts, 3);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.filter.categoryBits = GROUND;
		fd.filter.maskBits = PLAYER | TROLLS | SENSOR;	

		groundBody = world->CreateBody(&bd);
		groundBody->CreateFixture(&fd);
	}

	createLevelEdges();
}
void GameLayer::createLevelEdges()
{
	for (int i=0; i<2; i++)
	{
		b2BodyDef bd;
		bd.type = b2_staticBody;

		b2EdgeShape shape;
		shape.Set(b2Vec2(i*levelWidth/PTMR,1000), b2Vec2(i*levelWidth/PTMR, 0));

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.filter.categoryBits = LVLEDGE;
		fd.filter.maskBits = PLAYER|TROLLS|SENSOR;
	
		b2Body *edge = world->CreateBody(&bd);
		edge->CreateFixture(&fd);
	}
}

void GameLayer::update(float dt)
{
	followPlayer();
}
void GameLayer::followPlayer()
{
	float diff = position.x + player->position.x;

	if (diff < 150)
		position.x = -player->position.x + 150;
	else if (diff > 240)
		position.x = -player->position.x + 240;

	if (position.x > 0)
		position.x = 0;
	if (position.x < -levelWidth+384)
		position.x = -levelWidth+384;

	position.y = -player->position.y / 5.f;
}

void GameLayer::keyEvent(Pim::KeyEvent &evt)
{
}
