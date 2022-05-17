// Copyright Jonathan Carlson 2022

#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <iostream>
using namespace std;

#include "initializer.h"
#include "miscellaneous.h"

class GameMenu {
public:
public:
    GameMenu() {}
    GameMenu(Initializer & globalData); 

    void gameMenuLoop();

private:

    Initializer* globalData;
    vector<sf::Text> menuItemTexts;
    vector<pair<short, short>> menuItemPositions = {
        {200.f, 100.f},
        {200.f, 200.f},
        {200.f, 300.f}
    };


    void setMenuItemPositions();

};

#endif // GAMEMENU_H


// =================================================================================================


GameMenu::GameMenu(Initializer & globaData) {
    this->globalData = globalData;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::gameMenuLoop() {
    cout << "This is the menu!" << endl;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::setMenuItemPositions() {
    for(short i = 0; i < menuItemPositions.size(); i++) {
        menuItemTexts.push_back(sf::Text());
        menuItemTexts[i].setFont(globalData->defaultFont); 
        menuItemTexts[i].setCharacterSize(50); 
        menuItemTexts[i].setFillColor(sf::Color(255, 255, 255));
        menuItemTexts[i].setString("Some menu item");
        Miscellaneous::centerTextAlignment(menuItemTexts[i]);
        menuItemTexts[i].setPosition(sf::Vector2f(menuItemPositions[i].first, 
                                                  menuItemPositions[i].second));
    }
}