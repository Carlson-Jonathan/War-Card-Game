// Copyright Jonathan Carlson 2022

#ifndef GAME_H
#define GAME_H

#include <iostream>
using namespace std;

#include "cardTable.h"

class Game {
public:

    Game() {}
    Game(Initializer & globalData) : cardTable(globalData) {
        this->globalData = &globalData;
    }

    //----------------------------------------------------------------------------------------------

    void gameLoop() {
        while(globalData->window.isOpen()) {
            globalData->eventHandler.listen();
            globalData->window.clear(sf::Color(0, 90, 0));
            gameContent();
            globalData->window.display();
        } 
    }

private:

    Initializer* globalData;
    CardTable    cardTable;
    short        numberOfPlayers;


    void gameContent() {
        globalData->window.draw(cardTable.text);
        cardTable.drawCardsOnTable();
    }
};

#endif // GAME_H