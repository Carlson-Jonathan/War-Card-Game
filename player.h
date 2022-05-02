// Copyright Jonathan Carlson 2022

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
using namespace std;

#include "initializer.h"

class Player {
public:

    Player () {}

    Player* after  = NULL;
    Player* before = NULL;
    string  name   = "Unset";
    short   numCardsInHand;

    vector<Card> hand;
};

#endif // PLAYER_H


// =================================================================================================