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

        // for(short i = 0, j = -100, k = -50; i < cardDeck.deck.size(); i++, j -= 40, k -= 10) {
        //     cardDeck.deck[i].cardSprite.setOrigin(j, k);
        //     if(i == 25) {
        //         j = -60;
        //     }
	    // }

        cardDeck.deck[0].cardSprite.setOrigin(card_1_Origin.first, card_1_Origin.second);
        cardDeck.deck[1].cardSprite.setOrigin(card_2_Origin.first, card_2_Origin.second);
        cardDeck.deck[2].cardSprite.setOrigin(card_3_Origin.first, card_3_Origin.second);
        cardDeck.deck[3].cardSprite.setOrigin(card_4_Origin.first, card_4_Origin.second);        

        cardDeck.cardBacks[0].setOrigin(cardBacks_1_Origin.first, cardBacks_1_Origin.second);
        cardDeck.cardBacks[1].setOrigin(cardBacks_2_Origin.first, cardBacks_2_Origin.second);
        cardDeck.cardBacks[2].setOrigin(cardBacks_3_Origin.first, cardBacks_3_Origin.second);
        cardDeck.cardBacks[3].setOrigin(cardBacks_4_Origin.first, cardBacks_4_Origin.second);              
    }

    //----------------------------------------------------------------------------------------------

    void drawCardsOnTable() {
        // for(short i = 0; i < cardDeck.deck.size(); i++) {
		// 	globalData->window.draw(cardDeck.deck[i].cardSprite);
		// }

        for(short i = 0; i < 4; i++) {
            globalData->window.draw(cardDeck.deck[i].cardSprite);
            globalData->window.draw(cardDeck.cardBacks[i]);

        }
    }

private:

    Initializer* globalData;
    CardDeck     cardDeck; 
    string       fontFile = "Fonts/Robusta-Regular.ttf";
	sf::Font     font; 

    pair<short, short> card_1_Origin = {-491, -375};
    pair<short, short> card_2_Origin = {-741, -230};
    pair<short, short> card_3_Origin = {-566, -158};
    pair<short, short> card_4_Origin = {-666, -447};

    pair<short, short> cardBacks_1_Origin = {-491, -545};
    pair<short, short> cardBacks_2_Origin = {-741, -60};
    pair<short, short> cardBacks_3_Origin = {-441, -158};
    pair<short, short> cardBacks_4_Origin = {-791, -447};    

    // pair<short, short> cardBackOrigin = {-435, -302};
};

#endif // CARDTABLE_H