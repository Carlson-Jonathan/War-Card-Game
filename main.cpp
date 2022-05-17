// Copyright Jonathan Carlson 2022

#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <time.h>
#include "initializer.h"
#include "gameTable.h"
#include "gameMenu.h"

using namespace std;


int main() {

	srand(time(NULL)); 		// For seeding the random number generator
	Initializer         globalData;
	GameMenu  gameMenu (globalData);
	GameTable gameTable(globalData);

	sf::Sprite gearMenuIcon;
	gearMenuIcon.setTextureRect(sf::IntRect(0, 0, 30, 30));
    gearMenuIcon.setTexture(globalData.textures.textures["gearMenuIcon"]);
    gearMenuIcon.setOrigin(-10, -10);

	while(globalData.window.isOpen()) {
		globalData.eventHandler.listen();
		globalData.window.clear(sf::Color(0, 90, 0));


		// Toggle between game and menu
		if(globalData.gameMenuIsOpen)
			gameMenu.gameMenuLoop();
		else
			gameTable.gameTableLoop();

		if(globalData.eventHandler.menuIconWasClicked) {
			globalData.eventHandler.menuIconWasClicked = false;
			if(globalData.gameMenuIsOpen)
				globalData.gameMenuIsOpen = false;
			else
				globalData.gameMenuIsOpen = true;
		}


		globalData.window.draw(gearMenuIcon);
		globalData.window.display();
	} 

	return 0;
}

#endif // MAIN_CPP