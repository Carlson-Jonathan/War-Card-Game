// Copyright Jonathan Carlson 2022

#ifndef CARDTABLE_H
#define CARDTABLE_H

#include <iostream>
using namespace std;

#include "initializer.h"
#include "cardDeck.h"

class CardTable {
public:

    CardTable() {}
    CardTable(Initializer & globalData) : cardDeck(globalData) {
        this->globalData = &globalData;
        setDemoText();
        setCardPositions();
    }

    sf::Text text;

    //----------------------------------------------------------------------------------------------

    void setDemoText() {
        if (!font.loadFromFile(fontFile)) {
            cout << "Error: CardTable::setText(): Font " << fontFile << " not found." << endl; 
            exit(139);
        }

        text.setFont(font); 
        text.setString("Card Game Framework by Jonathan Carlson   |   (C) 2022");
        text.setCharacterSize(40); 
        text.setFillColor(sf::Color::Yellow);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top  + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(globalData->screenWidth / 1.65, 60));
    }

    //----------------------------------------------------------------------------------------------

    void setCardPositions() {
        for(short i = 0, j = -100, k = -50; i < cardDeck.deck.size(); i++, j -= 40, k -= 10) {
            cardDeck.deck[i].cardSprite.setOrigin(j, k);
            if(i == 25) {
                j = -60;
            }
	    }
    }

    //----------------------------------------------------------------------------------------------

    void drawCardsOnTable() {
        for(short i = 0; i < cardDeck.deck.size(); i++) {
			globalData->window.draw(cardDeck.deck[i].cardSprite);
		}
    }

private:

    Initializer* globalData;
    CardDeck     cardDeck; 
    string       fontFile = "Fonts/Robusta-Regular.ttf";
	sf::Font     font; 
};

#endif // CARDTABLE_H