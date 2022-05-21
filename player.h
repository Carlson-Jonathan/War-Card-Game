// Copyright Jonathan Carlson 2022

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#include "initializer.h"

class Player {
public:

    Player () {}

    Player* after       = NULL;
    Player* before      = NULL;
    string  name        = "Unset";
    short   number      = 0;
    bool    isOutOfGame = false;
    short   numCardsInHand;

    vector<shared_ptr<Card>> hand;
    shared_ptr<Card> topCard;
};


// =================================================================================================


#endif // PLAYER_H