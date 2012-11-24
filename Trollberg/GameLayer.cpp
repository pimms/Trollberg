#include "GameLayer.h"
#include "GameScene.h"
#include "Player.h"
#include "TrollControl.h"
#include "Troll.h"
#include "Rock.h"


GameLayer* GameLayer::singleton = NULL;

// STATICS
GameLayer* GameLayer::getSingleton()
{
	return singleton;
}
Pim::SpriteBatchNode* GameLayer::getActorSheet()
{
	return singleton->actorSheet;
}
bool GameLayer::isPlayerDead()
{
	return singleton->player->deathTimer >= 0.6f;
}


GameLayer::GameLayer(int lvl)
{
	levelNum		= lvl;
	world			= GameScene::getWorld();
	groundBody		= NULL;
	trollControl	= NULL;
	singleton		= this;
}
GameLayer::~GameLayer()
{
	if (trollControl)
	{
		delete trollControl;
	}

	Troll::destroyFont();

	singleton = NULL;
}

void GameLayer::draw()
{
	Pim::Layer::draw();

	//return;	// Comment the return to enable physics debug-drawing

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
			// Is the body a polygon shape?
			b2PolygonShape *poly = dynamic_cast<b2PolygonShape*>(f->GetShape());
			if (poly)
			{
				glPushMatrix();
				glTranslatef(b->GetPosition().x, b->GetPosition().y, 0.f);
				glRotatef(b->GetAngle()*RADTODEG, 0.f, 0.f, 1.f);

				glBegin(GL_POLYGON);
					for (int i=0; i<poly->GetVertexCount(); i++)
					{
						glVertex2f(poly->GetVertex(i).x, poly->GetVertex(i).y);
					}
				glEnd();

				glPopMatrix();
			}

			// Is the body a circular shape?
			b2CircleShape *circle = dynamic_cast<b2CircleShape*>(f->GetShape());
			if (circle)
			{
				glPushMatrix();
				glTranslatef(b->GetPosition().x, b->GetPosition().y, 0.f);
				
				float rad = circle->m_radius;

				glBegin(GL_TRIANGLE_FAN);
					glVertex2f(0.f, 0.f);

					for (float a=0.f; a<2*M_PI; a+=(2*M_PI)/32.f)
					{
						glVertex2f( rad*cosf(a), rad*sinf(a) );
					}

					glVertex2f( rad*cosf(0.f), rad*sinf(0.f) );
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

	actorSheet = new Pim::SpriteBatchNode("res\\actor.png");
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

	// Preload the Lighting Rifle bullet tex
	Pim::SmoothLightDef *sld = new Pim::SmoothLightDef;
	sld->radius = 250;
	sld->innerColor.a = 0.7f;
	preloadLightTexture(sld, "LRBullet");

	// Preoad the Colossus crush light
	Pim::FlatLightDef *fld = new Pim::FlatLightDef;
	fld->radius = 150;
	fld->falloff = 0.5f;
	preloadLightTexture(fld, "CCrush");
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
	trollControl->update(dt);
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
	if (evt.isKeyFresh(Pim::KeyEvent::K_ESC))
	{
		Pim::GameControl::getSingleton()->pause();
	}
}
