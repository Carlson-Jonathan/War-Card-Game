// Copyright Jonathan Carlson 2022

#ifndef CARD_TEST_H
#define CARD_TEST_H

#include <iostream>
using namespace std;

#include "../card.h"

class Card_Test {
public: 

    static bool setCardValue_test() {

        Card card;
        vector<string> cardNames = {
            "king_of_diamonds.png",
            "ace_of_hearts.png",
            "10_of_spades.png",
            "jack_of_clubs.png",
            "queen_of_hearts.png",
            "2_of_diamonds"
        };

        vector<short> expectedResults = {13, 14, 10, 11, 12, 2};

        for(short i = 0; i < cardNames.size(); i++) {
            card.setCardValue(cardNames[i]);
            if(card.value != expectedResults[i]) {
                cout << "Unit test failed: card_test::setCardValue_test(): Value not set correctly: '"
                     << cardNames[i] << "' value not equal to " << expectedResults[i] << endl;
                return false;
            }
        }

        return true;
    }

    //----------------------------------------------------------------------------------------------

    static bool setCardSuite_test() {

        Card card;
        vector<string> cardNames = {
            "king_of_diamonds",
            "ace_of_hearts",
            "10_of_spades",
            "jack_of_clubs",
            "queen_of_hearts",
            "2_of_diamonds"
        };

        vector<string> expectedResults = {"diamonds", "hearts", "spades", "clubs", "hearts", "diamonds"};

        for(short i = 0; i < cardNames.size(); i++) {
            card.setCardSuite(cardNames[i]);
            if(card.suite != expectedResults[i]) {
                cout << "Unit test failed: card_test::setCardSuite_test(): Suite not set correctly: '"
                     << cardNames[i] << "' Suite not equal to " << expectedResults[i] << " ('" << card.suite << "')\n";
                return false;
            }
        }

        return true;        
    }

};

#endif // CARD_TEST_H