// Copyright Jonathan Carlson 2022

#ifndef CARDDECK_H
#define CARDDECK_H

#include <iostream>
using namespace std;

#include "card.h"
#include "miscellaneous.h"

class CardDeck_Test;

class CardDeck {
public:
    friend CardDeck_Test;

    CardDeck() {}
    CardDeck(Initializer & globalData) {
        this->globalData = &globalData;
        this->cardNames = Miscellaneous::getAllFileNamesFromDirectory("Images/Objects/PlayingCards/100x150");
        shuffleDeck(this->cardNames);
        generateCardDeck();
        divideDeck(5);
    }

	vector<Card> deck;

    //----------------------------------------------------------------------------------------------

    vector<vector<Card>> divideDeck(short numberOfDecks) {

        vector<vector<Card>> decks;
        short deckSizes = this->deck.size() / numberOfDecks;
        short next = 0;

        // Deal into even decks.
        for(short i = 0; i < numberOfDecks; i++) { 
            decks.push_back(
                vector<Card>(this->deck.begin() + next, this->deck.begin() + deckSizes + next)
            );
            next += deckSizes;
        }            

        // Deal out left-over cards.
        for(short i = 0; i < deck.size() % deckSizes; i++) {
            decks[i].push_back(deck[i + (deckSizes * numberOfDecks)]);
        }

        return decks;
    }

    //----------------------------------------------------------------------------------------------

    void shuffleDeck(vector<string> cardNames) {
        short deckSize = cardNames.size();
        while(deckSize) {
            short index = Miscellaneous::generateRandomNumber(deckSize--) - 1;
            cardNames.push_back(cardNames[index]);
            cardNames.erase(cardNames.begin() + index);
        }
        this->cardNames = cardNames;
    }

    //----------------------------------------------------------------------------------------------

    void printDeck(vector<Card> deck) {
        for(auto i : deck) {
            cout << i.value << " of " << i.suite << endl;
        }
    }

private:

    vector<string> cardNames;
    Initializer*   globalData;

    //----------------------------------------------------------------------------------------------

    void generateCardDeck() {
        for(string & i : this->cardNames) {
            i = globalData->textures.getTextureName(i);
        }

        for(string i : this->cardNames) {
            deck.push_back(Card(*globalData, i));
        }
    }

};

#endif // CARDDECK_H