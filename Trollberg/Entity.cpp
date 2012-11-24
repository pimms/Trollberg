#include "Entity.h"
#include "GameScene.h"


Entity::Entity(void)
{
	body = NULL;
	world = GameScene::getWorld();
}

void Entity::draw()
{
	/*
		This method is copypasted from PimSprite's implementation.
		The b2offset of the entity is taken into account. That's the difference.
	*/

	glPushMatrix();

	// Update view matrix
	Pim::Vec2 fac = Pim::GameControl::getSingleton()->coordinateFactor();

	glTranslatef((position.x + b2offset.x) / fac.x, (position.y + b2offset.y) / fac.y, 0.f);

	glRotatef(rotation, 0.f, 0.f, 1.f);

	// Push matrix before scaling and overlaying color
	glPushMatrix();

	fac = Pim::GameControl::getSingleton()->windowScale();
	glScalef(scale.x * fac.x, scale.y * fac.y, 1.f);
	glColor4f(color.r, color.g, color.b, color.a);

	glBindTexture(GL_TEXTURE_2D, texID);

	if (shader)
	{
		shader->setUniform1i("texture", 0);
		glUseProgram(shader->getProgram());
	}

	if (!hidden)
	{
		// Counter clockwise 
		glBegin(GL_QUADS);
			glTexCoord2f((float)rect.x / (float)_tw, (float)rect.y / (float)_th);
			glVertex2f(-anchor.x * rect.width, -anchor.y * rect.height);

			glTexCoord2f((float)rect.x/(float)_tw + (float)rect.width/(float)_tw, (float)rect.y / (float)_th);
			glVertex2f((1.f-anchor.x) * rect.width, -anchor.y * rect.height);

			glTexCoord2f((float)rect.x/(float)_tw + (float)rect.width/(float)_tw, (float)rect.y/(float)_th + (float)rect.height/(float)_th);
			glVertex2f((1.f-anchor.x) * rect.width, (1.f-anchor.y) * rect.height);

			glTexCoord2f((float)rect.x/(float)_tw, (float)rect.y/(float)_th + (float)rect.height/(float)_th);
			glVertex2f(-anchor.x * rect.width, (1.f-anchor.y) * rect.height);
		glEnd();
	}
		
	glUseProgram(0);

	// Debug draw shadow shape if flagged to do so
	//if (shadowShape && dbgShadowShape)
	//	shadowShape->debugDraw();

	// THE INTEGRATED COLLISION DETECTION LIBRARY IS DEPRECATED
	//if (colShape && dbgColShape)
	//	colShape->debugDraw();

	// Children are unaffected by their parent's scale. Restore.
	glPopMatrix();

	orderChildren();
	for (unsigned int i=0; i<children.size(); i++)
	{
		children[i]->draw();
	}

	// Restore this parent's view matrix
	glPopMatrix();
}

void Entity::setPosition(Pim::Vec2 pos)
{
	position = pos;
	body->SetTransform(toB2(pos), body->GetAngle());
}
void Entity::setPosition(b2Vec2 pos)
{
	position = toPim(pos);
	body->SetTransform(pos, body->GetAngle());
}

b2Body* Entity::createRectangularBody(Pim::Vec2 dimensions, int category, int mask, float density)
{
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.fixedRotation		= true;
	bd.allowSleep			= false;
	bd.position				= toB2(position);

	b2PolygonShape shape;
	shape.SetAsBox(dimensions.x/PTMR/2.f, dimensions.y/PTMR/2.f);

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.f;
	fd.friction				= 0.f;
	fd.density				= (density)?(density):((dimensions.x/PTMR) * (dimensions.y/PTMR));
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	b2Body *retBody = world->CreateBody(&bd);
	retBody->CreateFixture(&fd);
	retBody->SetUserData(this);

	retBody->SetTransform(toB2(position), 0.f);

	return retBody;
}
b2Body* Entity::createCircularBody(float radius, int category, int mask, float density)
{
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.fixedRotation		= true;
	bd.allowSleep			= false;
	bd.position				= toB2(position);
	
	b2CircleShape shape;
	shape.m_radius			= radius / PTMR;

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.f;
	fd.friction				= 0.f;
	fd.density				= (density)?(density):(pow((radius/PTMR)*M_PI, 2));
	fd.userData				= this;
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	b2Body *retBody = world->CreateBody(&bd);
	retBody->CreateFixture(&fd);
	retBody->SetUserData(this);

	retBody->SetTransform(toB2(position), 0.f);

	return retBody;
}

void Entity::deleteBody()
{
	if (body)
	{
		world->DestroyBody(body);
		body = NULL;
	}
}

b2Fixture* Entity::otherCollidingFixture(b2Contact *c, unsigned int catFlags)
{
	if (c && c->IsTouching())
	{
		b2Fixture *ret = NULL;
		if (c->GetFixtureA() != body->GetFixtureList())
		{
			ret = c->GetFixtureA();
		}
		else
		{
			ret = c->GetFixtureB();
		}

		if ((ret->GetFilterData().categoryBits & catFlags) != 0)
		{
			return ret;
		}
	}

	return NULL;
}