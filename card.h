// Copyright Jonathan Carlson 2022

#ifndef CARD_H
#define CARD_H

#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "initializer.h"

class Card_Test;

class Card {
public:
    friend Card_Test;

    Card() {}
    Card(Initializer & globalData, string cardName) {
        this->globalData = &globalData;

        cardSprite.setTextureRect(sf::IntRect(0, 0, 100, 145));
 	    cardSprite.setTexture(globalData.textures.textures[cardName]);
		cardSprite.setOrigin(-100, -100);	

        setCardValue(cardName);
        setCardSuite(cardName);
    }

    short      value = 0;
    string     suite = "unset";
 	sf::Sprite cardSprite; 

    //----------------------------------------------------------------------------------------------

    void printCardInfo(string cardName) {
        cout << "[]" << endl;
        cout << cardName << endl;
        cout << "   Value: " << this->value << endl;
        cout << "   Suite: " << this->suite << endl;
    }

    //----------------------------------------------------------------------------------------------

private:

    Initializer*     globalData;
    map<char, short> faceCardValues = {{'j', 11}, {'q', 12}, {'k', 13}, {'a', 14}};

    //----------------------------------------------------------------------------------------------

    void setCardValue(string cardName) {
        string exception = "Error: Card::setCardValue(): Attempted to create a card using an invalid value: " + 
            to_string(this->value) + " ('" + cardName + "')";

        if(isdigit(cardName[0])) {
            if(cardName[0] == 49)
                this->value = 10;
            else {
                this->value = cardName[0] - 48;
                if(this->value < 2 || this->value > 14) {
                    cout << exception << endl;
                    exit(139);
                }
            }
        } else {
            if (faceCardValues.find(cardName[0]) == faceCardValues.end()) {
                cout << exception << endl;
                exit(139);
            }
            this->value = faceCardValues[cardName[0]];
        }
    }

    //----------------------------------------------------------------------------------------------

    void setCardSuite(string cardName) {
        vector<string> validSuites = {"clubs", "hearts", "diamonds", "spades"};
        bool invalidSuite = true;
        string exception = "Error: Card::setCardValue(): Attempted to create a card using an invalid suite: " + 
            this->suite + " ('" + cardName + "')";

        size_t found = cardName.find("f_");

        if (found != string::npos)
            this->suite = cardName.substr(found + 2);
        
        for(auto i : validSuites) {
            if(suite == i)
                invalidSuite = false;
        }

        if(invalidSuite) {
            cout << exception << endl;
            exit(139);
        }
    }
};

#endif // CARD_H