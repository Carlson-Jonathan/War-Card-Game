// Copyright Jonathan Carlson 2022

#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
using namespace std;

#include <time.h>
#include "initializer.h"
#include "gameTable.h"

int main() {

	srand(time(NULL)); 		// For seeding the random number generator
	Initializer globalData;

	GameTable gameTable(globalData);
	gameTable.gameLoop();

	return 0;
}

#endif // MAIN_CPP