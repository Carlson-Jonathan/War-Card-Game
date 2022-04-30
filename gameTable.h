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
        verifyNumberOfPlayers();
        this->globalData = &globalData;
        generatePlayers();
        dealCards();
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

        float xMid = -(globalData->screenWidth / 2.0);
        float yMid = -(globalData->screenHeight / 2.0);

        vector<pair<float, float>> cardPositions = {
            {xMid + 200.f, yMid + 170.f}, // Player 1
            {xMid +  50.f, yMid + 170.f}, // Player 2
            {xMid - 100.f, yMid + 170.f}, // Player 3
            {xMid + 200.f, yMid - 25.f},  // Player 4
            {xMid +  50.f, yMid - 25.f},  // Player 5
            {xMid - 100.f, yMid - 25.f}   // Player 6
        };

        for(short i = 0; i < numberOfPlayers; i++) {
            for(short j = 0; j < playerList[i]->hand.size(); j++) {
                playerList[i]->hand[j].cardSprite.setOrigin(cardPositions[i].first, cardPositions[i].second);
            }
        }

        printAllPlayerStats(cardPositions);

        cardDeck.cardBacks[0].setOrigin(xMid + 200, yMid + 340);              
        cardDeck.cardBacks[1].setOrigin(xMid +  50, yMid + 340);   
        cardDeck.cardBacks[2].setOrigin(xMid - 100, yMid + 340);   
        cardDeck.cardBacks[3].setOrigin(xMid + 200, yMid - 195);
        cardDeck.cardBacks[4].setOrigin(xMid +  50, yMid - 195);
        cardDeck.cardBacks[5].setOrigin(xMid - 100, yMid - 195);
    }

    //----------------------------------------------------------------------------------------------

    void drawCardsOnTable() {
        // for(short i = 0; i < cardDeck.deck.size(); i++) {
		// 	globalData->window.draw(cardDeck.deck[i].cardSprite);
		// }

        for(short i = 0; i < numberOfPlayers; i++) {
            globalData->window.draw(playerList[i]->hand[0].cardSprite);
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

    //----------------------------------------------------------------------------------------------

    void dealCards() {
        vector<vector<Card>> hands;
        hands = cardDeck.divideDeck(numberOfPlayers);
        for(short i = 0; i < numberOfPlayers; i++) {
            playerList[i]->hand = hands[i];
        }
    }

    //----------------------------------------------------------------------------------------------

    void verifyNumberOfPlayers() {
        if(numberOfPlayers < 2 || numberOfPlayers > 6) {
            cout << "ERROR: GameTable::numberOfPlayers: Out of range. Value must be 2-6." << endl;
            exit(139);
        }
    }

    //----------------------------------------------------------------------------------------------

    void printAllPlayerStats(vector<pair<float, float>> cardPositions) {
        for(short i = 0; i < numberOfPlayers; i++) {
            cout << "\n==============================\n   Player " << i + 1 << " (Hand size = " 
                 << playerList[i]->hand.size() << ")\n==============================\n";
            for(short j = 0; j < playerList[i]->hand.size(); j++) {
                cout << playerList[i]->hand[j].cardName << "\t  {" << cardPositions[i].first << ", " 
                     << cardPositions[i].second << "}" << endl;
            }
        }
    }


private:

    LinkedList<Player> playerList;
    short numberOfPlayers = 5;
    Initializer* globalData;
    CardDeck     cardDeck; 
    string       fontFile = "Fonts/Robusta-Regular.ttf";
	sf::Font     font; 
};

#endif // GAMETABLE_H