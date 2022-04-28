// Copyright Jonathan Carlson 2022

#ifndef GAME_H
#define GAME_H

#include <iostream>
using namespace std;

#include "cardTable.h"
#include "linkedList.h"

class Game {
public:

    Game() {}
    Game(Initializer & globalData) : cardTable(globalData) {
        this->globalData = &globalData;
        generatePlayers();
        playerList.printList();
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

    //----------------------------------------------------------------------------------------------

private:

    Initializer* globalData;
    CardTable    cardTable;

    short numberOfPlayers = 6;
    LinkedList<Player> playerList;

    //----------------------------------------------------------------------------------------------

    void gameContent() {
        // globalData->window.draw(cardTable.text);
        cardTable.drawCardsOnTable();
    }

    //----------------------------------------------------------------------------------------------

    void generatePlayers() {
        for(short i = 0; i < numberOfPlayers; i++) {
            playerList.insert(new Player, i);
            playerList.getElement(i)->name = "Player " + to_string(i + 1);
        }
        playerList.makeListCircular();
    }
};

#endif // GAME_H