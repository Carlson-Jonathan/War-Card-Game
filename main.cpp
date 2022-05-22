// Copyright Jonathan Carlson 2022

#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <time.h>
#include "initializer.h"
#include "gameTable.h"
#include "gameMenu.h"
#include "titleScreen.h"

using namespace std;

int main() {

	srand(time(NULL)); 		// For seeding the random number generator
	Initializer globalData;
	TitleScreen titleScreen (globalData);
	GameMenu    gameMenu    (globalData);
	GameTable   gameTable;

	while(globalData.window.isOpen()) {
		globalData.eventHandler.listen();
		globalData.window.clear(sf::Color(0, 90, 0));

		if(globalData.mayInitializeGameTable) {
			globalData.mayInitializeGameTable = false;
			gameMenu.updateNumberOfPlayersText(globalData.numberOfPlayers);
			gameTable.construct(globalData);
		}

		// Toggle between game and menu
		if(globalData.atTitleScreen)
			titleScreen.titleScreenLoop();
		else if(globalData.gameMenuIsOpen)
			gameMenu.gameMenuLoop();
		else
			gameTable.gameTableLoop();

		globalData.window.display();
	} 

	return 0;
}

#endif // MAIN_CPP