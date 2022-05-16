// Copyright Jonathan Carlson 2022

#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <iostream>
using namespace std;

#include "initializer.h"

class GameMenu {
public:
public:
    GameMenu() {}
    GameMenu(Initializer & globalData); 

    void gameMenuLoop();

private:

    Initializer* globalData;
};

// =================================================================================================


GameMenu::GameMenu(Initializer & globaData) {
    this->globalData = globalData;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::gameMenuLoop() {
    cout << "This is the menu!" << endl;
}

// -------------------------------------------------------------------------------------------------

#endif // GAMEMENU_H