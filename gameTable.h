// Copyright Jonathan Carlson 2022

#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <iostream>
using namespace std;

#include "initializer.h"
#include "cardDeck.h"
#include "linkedList.h"
#include "player.h"

class GameTable {
public:

    GameTable() {}
    GameTable(Initializer & globalData) : cardDeck(globalData) {
        this->globalData = &globalData;
        generatePlayers();
        setCardPositions();
    }

    sf::Text text;

    //----------------------------------------------------------------------------------------------

    void setDemoText() {
        if (!font.loadFromFile(fontFile)) {
            cout << "Error: GameTable::setText(): Font " << fontFile << " not found." << endl; 
            exit(139);
        }

        text.setFont(font); 
        text.setString("War Card Game by Jonathan Carlson   |   (C) 2022");
        text.setCharacterSize(40); 
        text.setFillColor(sf::Color::Yellow);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top  + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(globalData->screenWidth / 1.65, 60));
    }

    //----------------------------------------------------------------------------------------------

    void setCardPositions() {

        // for(short i = 0, j = -100, k = -50; i < cardDeck.deck.size(); i++, j -= 40, k -= 10) {
        //     cardDeck.deck[i].cardSprite.setOrigin(j, k);
        //     if(i == 25) {
        //         j = -60;
        //     }
	    // }

        short xMid = -(globalData->screenWidth / 2);
        short yMid = -(globalData->screenHeight / 2);

        cardDeck.deck[0].cardSprite.setOrigin(xMid + 200, yMid - 25);
        cardDeck.deck[1].cardSprite.setOrigin(xMid +  50, yMid - 25);
        cardDeck.deck[2].cardSprite.setOrigin(xMid - 100, yMid - 25);
        cardDeck.deck[3].cardSprite.setOrigin(xMid + 200, yMid + 170);
        cardDeck.deck[4].cardSprite.setOrigin(xMid +  50, yMid + 170);
        cardDeck.deck[5].cardSprite.setOrigin(xMid - 100, yMid + 170);

        cardDeck.cardBacks[0].setOrigin(xMid + 200, yMid - 195);
        cardDeck.cardBacks[1].setOrigin(xMid +  50, yMid - 195);
        cardDeck.cardBacks[2].setOrigin(xMid - 100, yMid - 195);
        cardDeck.cardBacks[3].setOrigin(xMid + 200, yMid + 340);              
        cardDeck.cardBacks[4].setOrigin(xMid +  50, yMid + 340);   
        cardDeck.cardBacks[5].setOrigin(xMid - 100, yMid + 340);                   
    }

    //----------------------------------------------------------------------------------------------

    void drawCardsOnTable() {
        // for(short i = 0; i < cardDeck.deck.size(); i++) {
		// 	globalData->window.draw(cardDeck.deck[i].cardSprite);
		// }

        for(short i = 0; i < 6; i++) {
            globalData->window.draw(cardDeck.deck[i].cardSprite);
            globalData->window.draw(cardDeck.cardBacks[i]);
        }
    }

    //----------------------------------------------------------------------------------------------

    void generatePlayers() {
        for(short i = 0; i < numberOfPlayers; i++) {
            playerList.push_back(new Player);
            playerList[i]->name = "Player " + to_string(i + 1);
        }
        playerList.makeListCircular();
    }

    //----------------------------------------------------------------------------------------------

    void gameLoop() {
        while(globalData->window.isOpen()) {
            globalData->eventHandler.listen();
            globalData->window.clear(sf::Color(0, 90, 0));
            drawCardsOnTable();
            globalData->window.display();
        } 
    }    

private:

    LinkedList<Player> playerList;
    short numberOfPlayers = 6;
    Initializer* globalData;
    CardDeck     cardDeck; 
    string       fontFile = "Fonts/Robusta-Regular.ttf";
	sf::Font     font; 
};

#endif // GAMETABLE_H