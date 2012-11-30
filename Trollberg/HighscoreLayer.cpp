#pragma once
#include "HighscoreLayer.h"
#include "Trollberg.h"
#include "HUDLayer.h"
#include "MenuButton.h"
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameLayer.h"



HighscoreLayer::HighscoreLayer(bool onlyShow)
{

	this->onlyShow = onlyShow;

	arial40 = NULL;
	tittleLabel = NULL;

	listenFrame();
	listenInput();

	numData = 0;

	scoreStored = false;
	nameEntered = false;
	
	yourName = "";

}


HighscoreLayer::~HighscoreLayer(void)
{
	if(arial40)
	{
		delete arial40;
	}


}

void HighscoreLayer::loadResources()
{

	this->setZOrder(100);

	if (!arial40)
	{
		arial40 = new Pim::Font("res\\arial.ttf", 40, false);
	}

	

	//load bg
	Pim::Sprite *myBGImage = new Pim::Sprite("res/HIGHSCOREMENU.png");
	//testy->scale = Pim::Vec2(1.5f, 1.f);
	addChild(myBGImage);

	// Load the score label
	tittleLabel = new Pim::Label(arial40);
	tittleLabel->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
	tittleLabel->position = Pim::Vec2(0.f, +SCREENHEIGHT/2-30);
	tittleLabel->scale *= 0.3f;
	tittleLabel->setText("The famous High Score list");
	addChild(tittleLabel);

	if(!onlyShow)
	{
		yourScoreLabel = new Pim::Label(arial40);
		yourScoreLabel->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
		yourScoreLabel->position = Pim::Vec2(0.f, +SCREENHEIGHT/2-50);
		yourScoreLabel->scale *= 0.2f;
		yourScoreLabel->setTextWithFormat("You got: %d\n points", HUDLayer::getSingleton()->score);
		addChild(yourScoreLabel);
	

		youNameInputLabel = new Pim::Label(arial40);
		youNameInputLabel->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
		youNameInputLabel->position = Pim::Vec2(0.f, +SCREENHEIGHT/2-70);
		youNameInputLabel->scale *= 0.2f;
		youNameInputLabel->setText("Enter your name");
		addChild(youNameInputLabel);
	}

	//add all lables, and hide them
	for(int i = 0; i < HS_MAXLABELS; i ++)
	{
		theLables[i] = new Pim::Label(arial40);
		theLables[i]->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
		theLables[i]->scale *= 0.3f;
		addChild(theLables[i]);
	}

	//exit button
	Pim::SpriteBatchNode	*buttonSheet = new Pim::SpriteBatchNode("res/HSExit.png");
	Pim::Sprite *normal = new Pim::Sprite;
	normal->useBatchNode(buttonSheet) ; 
	normal->rect = Pim::Rect(0,0,20,20);

	Pim::Sprite *hovered = new Pim::Sprite;
	hovered->useBatchNode(buttonSheet);
	hovered->rect = Pim::Rect(20,0,20,20);

	Pim::Sprite *pressed = new Pim::Sprite;
	pressed->useBatchNode(buttonSheet); 
	pressed->rect = Pim::Rect(40,0,20,20);

	Pim::Button *closeButton = new Pim::Button(normal, hovered, pressed);
	addChild(closeButton);
	closeButton->position = Pim::Vec2(162.f, 80.f);
	closeButton->setCallback(this);

	updateScoreList();


}

void HighscoreLayer::buttonReleased(Pim::Button *activeButton){

	if (parent) //if parent, er dette en child
	{
		parent->removeChild(this, true);
	}
	else
	{
		//((MainMenuScene*)parentScene)->mml->setHighscoreDone();
		parentScene->removeLayer(this);
	}


}

//get content of xml doc and update lables:
void HighscoreLayer::updateScoreList()
{
	TiXmlDocument doc("highscore.pim");
	int counter = 0;

	//if we are able to load the file
	if(doc.LoadFile())
	{
		TiXmlHandle hDoc(&doc);
		TiXmlElement *pRoot, *pParm;
		pRoot = doc.FirstChildElement("palyers");
		if(pRoot)
		{
			pParm = pRoot->FirstChildElement("player");
			int i = 0; // for sorting the entries
			//max 6:
			while(pParm && numData < HS_MAXNUMHS-(!nameEntered))
			{

				std::string name = pParm->Attribute("name");
				std::string score = pParm->Attribute("score");

				//her er magien
				if(counter < HS_MAXLABELS)
				{
					updateText(0, 15 - (i*20), i, name + " got: " + score);
				}

				//her er litt Frodekode:
				if(!scoreStored)
				{
					strcpy(lastFileData[i][0], pParm->Attribute("name"));
					strcpy(lastFileData[i][1], pParm->Attribute("score"));
					numData ++;
				}


				pParm = pParm->NextSiblingElement("player");
				i++;
				counter ++;
			}
		}
	}

	scoreStored = true;

	

}

void HighscoreLayer::addMyScore()
{
	//må lese inn forrige og legge inn nytt!


	TiXmlDocument doc;  
	TiXmlElement* msg;

	//doc.FirstChildElement("palyers");
 
	TiXmlElement * root = new TiXmlElement( "palyers" );  
	doc.LinkEndChild( root );  

	TiXmlElement * window;
	window = new TiXmlElement( "player" );  
	root->LinkEndChild( window );
	const char *myName = yourName.c_str();
	window->SetAttribute("name", myName);
	window->SetAttribute("score", HUDLayer::getSingleton()->score);



	for(int i = 0; i < numData; i ++)
	{
		//vi liker vår frodekode!
		//den legger til de elementene fra filen da den ble lest til den nye
		window = new TiXmlElement( "player" );  
		root->LinkEndChild( window ); 
		window->SetAttribute("name", lastFileData[i][0]);
		window->SetAttribute("score", lastFileData[i][1]);

		//std::cout << "|" << lastFileData[i][0] << "|" << lastFileData[i][1] << "|" << i <<    "\n";

	}

	doc.SaveFile( "highscore.pim" );

	youNameInputLabel->setText("Saved!");

}

void HighscoreLayer::updateText(int xPos, int yPos, int index,  std::string text)
{
	theLables[index]->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
	theLables[index]->position = Pim::Vec2(xPos, yPos);

	theLables[index]->setText(text);

}

void HighscoreLayer::update(float dt)
{

}

void HighscoreLayer::keyEvent(Pim::KeyEvent &evt){

	if(onlyShow)
	{
		return;
	}

	for (int i = Pim::KeyEvent::K_A; i <= Pim::KeyEvent::K_Z && !nameEntered; i ++)
	{

		if (evt.isKeyFresh( (Pim::KeyEvent::KeyCode)i ) && yourName.size() <= HS_MAXNAMESIZE )
		{
			yourName += (char) i;
			youNameInputLabel->setText(yourName);
		}

	}

	if (evt.isKeyFresh(Pim::KeyEvent::KeyCode::K_BACKSPACE) && !nameEntered)
	{
		if(yourName.size() > 0)
		{
			yourName = yourName.substr(0, yourName.size()-1);
			youNameInputLabel->setText(yourName);
		}

	}

	if (evt.isKeyFresh(Pim::KeyEvent::KeyCode::K_ENTER) && !nameEntered)
	{
		nameEntered = true;
		addMyScore();
		updateScoreList();
	}


}

void HighscoreLayer::mouseEvent(Pim::MouseEvent &evt)
{
	if (evt.isKeyFresh(Pim::MouseEvent::MBTN_LEFT))
	{


	}

	if (evt.isKeyFresh(Pim::MouseEvent::MBTN_RIGHT))
	{
		updateScoreList();
		yourScoreLabel->setTextWithFormat("You got: %d\n points", HUDLayer::getSingleton()->score);
	}


}