#pragma once
#include "HighscoreLayer.h"
#include "Trollberg.h"
#include "HUDLayer.h"


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

}

void HighscoreLayer::loadResources()
{

	if (!arial40)
	{
		arial40 = new Pim::Font("res\\arial.ttf", 40, false);
	}

	//load bg
	Pim::Sprite *testy = new Pim::Sprite("res/HIGHSCOREMENU.png");
	//testy->scale = Pim::Vec2(1.5f, 1.f);
	addChild(testy);

	// Load the score label
	tittleLabel = new Pim::Label(arial40);
	tittleLabel->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
	tittleLabel->position = Pim::Vec2(0.f, +SCREENHEIGHT/2-30);
	tittleLabel->scale *= 0.3f;
	tittleLabel->setText("The famous High Score list");
	addChild(tittleLabel);

	if(!onlyShow)
	{
		youText = new Pim::Label(arial40);
		youText->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
		youText->position = Pim::Vec2(0.f, +SCREENHEIGHT/2-50);
		youText->scale *= 0.2f;
		youText->setTextWithFormat("You got: %d\n points", HUDLayer::getSingleton()->score);
		addChild(youText);
	

		youName = new Pim::Label(arial40);
		youName->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
		youName->position = Pim::Vec2(0.f, +SCREENHEIGHT/2-70);
		youName->scale *= 0.2f;
		youName->setText("Enter your name");
		addChild(youName);
	}

	//add all lables, and hide them
	for(int i = 0; i < HS_MAXLABELS; i ++)
	{
		theLables[i] = new Pim::Label(arial40);
		theLables[i]->color = Pim::Color(0.f, 0.f, 0.f, 1.f);
		theLables[i]->scale *= 0.3f;
		addChild(theLables[i]);
	}


	

	updateScoreList();


}

//get content of xml doc and update lables:
void HighscoreLayer::updateScoreList(){

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
			while(pParm)
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

void HighscoreLayer::addMyScore(){

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
	window->SetAttribute("score", 50);



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

	youName->setText("Saved!");

}

void HighscoreLayer::updateText(int xPos, int yPos, int index,  std::string text){

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

		if (evt.isKeyFresh( (Pim::KeyEvent::KeyCode)i ))
		{
			std::cout << (char) i <  "!\n";
			yourName += (char) i;
			youName->setText(yourName);
		}

	}

	if (evt.isKeyFresh(Pim::KeyEvent::KeyCode::K_ENTER) && !nameEntered)
	{
		nameEntered = true;
		std::cout << "\n" << "suksess!";
		addMyScore();
		updateScoreList();
	}


}

void HighscoreLayer::mouseEvent(Pim::MouseEvent &evt){

	if (evt.isKeyFresh(Pim::MouseEvent::MBTN_LEFT))
	{
		parent->removeChild(this, true);
		
	}

	if (evt.isKeyFresh(Pim::MouseEvent::MBTN_RIGHT))
	{
		updateScoreList();
		youText->setTextWithFormat("You got: %d\n points", HUDLayer::getSingleton()->score);
	}


}