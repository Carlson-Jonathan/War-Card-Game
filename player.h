#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
using namespace std;

#include "initializer.h"
#include "cardDeck.h"

class Player {
public:

    Player () {}

    Player* after = NULL;
    Player* before = NULL;

    CardDeck cardDeck;
    string name = "Unset";

};

#endif // PLAYER_H