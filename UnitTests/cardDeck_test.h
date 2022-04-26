// Copyright Jonathan Carlson 2022

#ifndef CARDDECK_TEST_H
#define CARDDECK_TEST_H

#include <iostream>
using namespace std;

#include <memory>
#include <vector>
#include "../cardDeck.h"
#include "../card.h"

class CardDeck_Test {
public:

    static bool shuffleDeck_test() {

        vector<string> cardNames = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
        CardDeck cardDeck;

        cardDeck.shuffleDeck(cardNames);

        if(cardDeck.cardNames == cardNames) {
            cout << "==============================================================================="
                 << endl;             
            cout << "Unit test failed: CardDeck::shuffle_test: Vector was not randomized!" << endl;
            for(auto i : cardDeck.cardNames) {
                cout << i << ", ";
            }
            cout << endl;
            return false;
        }

        return true;
    }
    
    //----------------------------------------------------------------------------------------------

    static bool divideDeck_test() {

        CardDeck cardDeck;
        cardDeck.deck = generateDeck();
        vector<vector<Card>> decks = cardDeck.divideDeck(7);
        vector<short> sizeResults = {8, 8, 8, 7, 7, 7, 7};
        set<short> duplicateValueChecker;
        set<string> duplicateSuiteChecker;

        // Verify number of cards is being distributed correctly.
        for(short i = 0; i < decks.size(); i++) {
            if(decks[i].size() != sizeResults[i]) {
                cout << "==============================================================================="
                     << endl;                
                cout << "Unit test failed: CardDeck::divideDeck_test: " 
                     << "Cards are not being distributed between split decks correctly." << endl;
                cardDeck.printDeck(cardDeck.deck);
                return false;
            }
        }

        // Verify all cards in the decks are unique.
        for(short i = 0; i < decks.size(); i++) {
            for(short j = 0; j < decks[i].size(); j++) {
                duplicateValueChecker.insert(decks[i][j].value);
            }
        }

        for(short i = 0; i < decks.size(); i++) {
            for(short j = 0; j < decks[i].size(); j++) {
                duplicateSuiteChecker.insert(decks[i][j].suite);
            }
        }        

        if(duplicateValueChecker.size() < cardDeck.deck.size() || 
           duplicateSuiteChecker.size() < cardDeck.deck.size()) {
            cout << "==============================================================================="
                 << endl;
            cout << "Unit test failed: CardDeck::divideDeck_Test: " 
                    << "Duplicate cards are being added to divided decks!" << endl;
            cout << "deck size = " << cardDeck.deck.size() << endl;
            cout << "duplicateValueChecker.size() = " << duplicateValueChecker.size() << endl;
            cout << "duplicateSuiteChecker.size() = " << duplicateSuiteChecker.size() << endl;
            cardDeck.printDeck(cardDeck.deck);
            return false;
        }

        return true;
    }

    //----------------------------------------------------------------------------------------------

private:

    static vector<Card> generateDeck() {
        CardDeck cardDeck;
        Card card;

        for(short i = 0; i < 52; i++) {
            card.value = i;
            card.suite = char(i + 65);
            cardDeck.deck.push_back(card);
        }

        return cardDeck.deck;
    }


};


#endif // CARDDECK_TEST_H