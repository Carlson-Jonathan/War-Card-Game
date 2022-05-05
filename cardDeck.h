// Copyright Jonathan Carlson 2022

#ifndef CARDDECK_H
#define CARDDECK_H

#include <iostream>
#include "card.h"
#include "miscellaneous.h"

using namespace std;

class CardDeck_Test; // For unit testing

class CardDeck {
public:

    CardDeck() {}
    CardDeck(Initializer & globalData);
    friend CardDeck_Test; // For unit testing

	vector<Card>         deck;
    vector<sf::Sprite>   cardBacks;
    vector<vector<Card>> divideDeck  (short numberOfDecks);
    void                 shuffleDeck ();
    void                 printDeck   (vector<Card> deck);

private:

    short          numberOfCardBacks = 6;
    Initializer*   globalData;
    vector<string> cardNames;

    void generateCardDeck  ();
    void generateCardBacks (short numberOfBacks);
};

#endif // CARDDECK_H


// =================================================================================================


CardDeck::CardDeck(Initializer & globalData) {
    this->globalData = &globalData;
    this->cardNames = Miscellaneous::getAllFileNamesFromDirectory("Images/Objects/PlayingCards/100x150");
    shuffleDeck();
    generateCardDeck();
    generateCardBacks(numberOfCardBacks);
}

// -------------------------------------------------------------------------------------------------

vector<vector<Card>> CardDeck::divideDeck(short numberOfDecks) {

    vector<vector<Card>> hands;
    short deckSizes = this->deck.size() / numberOfDecks;
    short next = 0;

    // Deal into even decks.
    for(short i = 0; i < numberOfDecks; i++) { 
        hands.push_back(
            vector<Card>(this->deck.begin() + next, this->deck.begin() + deckSizes + next)
        );
        next += deckSizes;
    }            

    // Deal out left-over cards.
    for(short i = 0; i < deck.size() % deckSizes; i++) {
        hands[i].push_back(deck[i + (deckSizes * numberOfDecks)]);
    }

    return hands;
}

// -------------------------------------------------------------------------------------------------

void CardDeck::shuffleDeck() {
    short deckSize = cardNames.size();
    while(deckSize) {
        short index = Miscellaneous::generateRandomNumber(deckSize--) - 1;
        cardNames.push_back(cardNames[index]);
        cardNames.erase(cardNames.begin() + index);
    }
    this->cardNames = cardNames;
}

// -------------------------------------------------------------------------------------------------

void CardDeck::printDeck(vector<Card> deck) {
    for(auto i : this->deck) {
        cout << i.value << " of " << i.suite << endl;
    }
}

// -------------------------------------------------------------------------------------------------

void CardDeck::generateCardDeck() {
    for(string & i : this->cardNames) {
        i = globalData->textures.getTextureName(i);
    }

    for(string i : this->cardNames) {
        this->deck.push_back(Card(*globalData, i));
    }
}

// -------------------------------------------------------------------------------------------------

void CardDeck::generateCardBacks(short numberOfBacks) {
    for(short i = 0; i < numberOfBacks; i++) {
        sf::Sprite cardBack;
        cardBack.setTexture(globalData->textures.textures["cardBack"]);
        this->cardBacks.push_back(cardBack);
    }
}