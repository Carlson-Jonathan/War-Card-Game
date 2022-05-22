// Copyright Jonathan Carlson 2022

#ifndef CARDDECK_H
#define CARDDECK_H

#include <iostream>
#include <memory>
#include "card.h"
#include "miscellaneous.h"

using namespace std;

class CardDeck_Test; // For unit testing

class CardDeck {
public:

    CardDeck() {}
    CardDeck(Initializer & globalData);
    friend CardDeck_Test; // For unit testing

	vector<shared_ptr<Card>> deck;
    vector<sf::Sprite> cardBacks;
    vector<vector<shared_ptr<Card>>> divideDeck(short numberOfDecks);

    void generateCardBacks (short numberOfBacks);
    void printDeck (vector<shared_ptr<Card>> deck);
    void shuffleDeck();

    friend bool operator<(const Card & l, const Card & r);
    friend bool operator>(const Card & l, const Card & r);

private:

    short          numberOfCardBacks = 6;
    Initializer*   globalData;
    vector<string> cardNames;

    void generateCardDeck  ();
};


// =================================================================================================


CardDeck::CardDeck(Initializer & globalData) {
    this->globalData = &globalData;
    this->cardNames = Miscellaneous::getAllFileNamesFromDirectory("Images/Objects/PlayingCards/100x150");
    shuffleDeck();
    generateCardDeck();
    generateCardBacks(numberOfCardBacks);
}

// -------------------------------------------------------------------------------------------------

vector<vector<shared_ptr<Card>>> CardDeck::divideDeck(short numberOfDecks) {

    vector<vector<shared_ptr<Card>>> hands;
    short deckSizes = this->deck.size() / numberOfDecks;
    short next = 0;

    // Deal into even decks.
    for(short i = 0; i < numberOfDecks; i++) { 
        hands.push_back(
            vector<shared_ptr<Card>>(this->deck.begin() + next, this->deck.begin() + deckSizes + next)
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

void CardDeck::printDeck(vector<shared_ptr<Card>> deck) {
    for(auto i : this->deck) {
        cout << i->value << " of " << i->suite << endl;
    }
}

// -------------------------------------------------------------------------------------------------

void CardDeck::generateCardDeck() {
    for(string & i : this->cardNames) {
        i = globalData->textures.getTextureName(i);
    }

    for(string i : this->cardNames) {
        shared_ptr<Card> newCard = make_shared<Card>(*globalData, i);
        this->deck.push_back(newCard);
    }
}

// -------------------------------------------------------------------------------------------------

void CardDeck::generateCardBacks(short numberOfBacks) {
    cardBacks = {};
    for(short i = 0; i < numberOfBacks; i++) {
        sf::Sprite cardBack;
        cardBack.setTexture(globalData->textures.textures[globalData->cardBack]);
        this->cardBacks.push_back(cardBack);
    }
}

// -------------------------------------------------------------------------------------------------

bool operator<(const Card & l, const Card & r) {
    return l.cardName < r.cardName;
}

// -------------------------------------------------------------------------------------------------

bool operator>(const Card & l, const Card & r) {
    return l.cardName > r.cardName;
}

#endif // CARDDECK_H