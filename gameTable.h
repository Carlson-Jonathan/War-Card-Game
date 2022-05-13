// Copyright Jonathan Carlson 2022

#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <iostream>
#include <vector>
#include <memory>
#include <set>
#include <assert.h>
#include "cardDeck.h"
#include "initializer.h"
#include "player.h"
#include "jc.h"

using namespace std;

class GameTable {
public:

    GameTable() {}
    GameTable(Initializer & globalData);
    void runGameLoop();

    short gameSpeed       = 0;
    short numberOfPlayers = 6;
    bool  autoClick       = true;

private:

    short faceupCards = 0;
    short timeElapsed = 0;
    short tieRound = 0;
    short tieBreakerIndex = 1;
    short placementDelay = 150;
    short resultsDelay = 1000;
    short conclusionDelay = 3000;
    short tieDelay = 2500;

    string fontFile = "Fonts/Robusta-Regular.ttf";
    Initializer* globalData;
    CardDeck cardDeck; 
    float xMid, yMid;

    vector<shared_ptr<Player>> playerList;
    vector<pair<float, float>> cardPositions;
    vector<shared_ptr<Player>> winnerPool;
    vector<shared_ptr<Card>>   prizePot;
    vector<jc::Sprite>         greenRectangles;
    vector<jc::Text>           handSizeNumbers;
    vector<jc::Text>           winnerText;
    vector<jc::Text>           tieText;

    bool mayClickOnCard            = true;
    bool mayStartNewRound          = false;
    bool allPlayersPlayedACard     = false;
    bool mayDeclareRoundResults    = false;
    bool hiPlayersNotYetDetermined = false;
    bool mayDisplayWinnerText      = false;
    bool mayDisplayTieText         = false;
    bool isATie                    = false;
    bool mayBreakTie               = false;
    bool mayConcludeRound          = false;
    bool gameOver                  = false;

    // For debugging:
    bool mayKickPlayer    = false;
    bool mayForceTie      = false;
    bool testGameEnding   = false;
    bool testLastCardTie  = false;
    
	sf::Font  font; 
    jc::Text  headingText;
    sf::Clock clock;
    sf::Time  elapsed; 

    // ---------------------------------------------------------------

    void setFontFamily();
    void setHeadingText();
    void setAndPlaceVictoryText();
    void setCardPositions();
    void setGameSpeed(short speed);
    void setGreenRectanglePositions();
    void setAndPlaceDeckNumberText();
    void setAndPlaceTieText();
    void centerTextAlignment(jc::Text & someText);

    void verifyNumberOfPlayers();
    void generatePlayers();
    void dealCardsToPlayers();    
    void drawCardsBacksAndNumbers();
    void drawAllTableSprites();
    vector<short> getActivePlayerIndecies();

    void activateGameRound();
    void revealFaceUpCardsWithDelayOf(short delay);
    void drawPlayerFaceUpCards();
    void adjustHandSizeNumber(shared_ptr<Player> player);

    short getHighestCardValuePlayed(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void fillWinnerPool(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void declareRoundResults();
    void determineWinnerOrTie();
    void breakTie();
    void breakTieOnLastCard();
    void playTieBreakerCard(shared_ptr<Player> player);

    void gatherPlayedCards();
    void popPlayedCardsFromPlayerDecks();
    void awardPrizePotToVictor();
    void concludeRound();
    void clearFaceUpCards();
    void resetRoundVariables();
    void removeDefeatedPlayers();
    void checkForGameWinner();

    void eventMonitor();
    void checkForMouseClickOnCard();
    void printAllPlayerStats();
    void printAllBooleanPermissions();
    void printPrizePotContents();
    void printPlayerMove(shared_ptr<Player> player);
    void scanForDuplicateCards();
    void verifyCardsEqualDeck();
    void kickPlayer(short playerNumber);
    void forceTie(shared_ptr<Player> player);
};

#endif // GAMETABLE_H


// =================================================================================================


GameTable::GameTable(Initializer & globalData) : cardDeck(globalData) {
    this->globalData = &globalData;
    verifyNumberOfPlayers();
    generatePlayers();
    setGameSpeed(gameSpeed);
    dealCardsToPlayers();
    setFontFamily();
    setCardPositions();
    setAndPlaceVictoryText();
    setGreenRectanglePositions();
    setAndPlaceDeckNumberText();
    if(mayKickPlayer) {
        // kickPlayer(1);
        // kickPlayer(2);
        // kickPlayer(6);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::setHeadingText() {

    headingText.setFont(font); 
    headingText.setString("War Card Game by Jonathan Carlson   |   (C) 2022");
    headingText.setCharacterSize(40); 
    headingText.setFillColor(sf::Color::Yellow);
    headingText.setPosition(sf::Vector2f(globalData->screenWidth / 1.65, 60));
    centerTextAlignment(headingText);
}

// -------------------------------------------------------------------------------------------------

void GameTable::centerTextAlignment(jc::Text & someText) {
    sf::FloatRect textRect = someText.getLocalBounds();
    someText.setOrigin(textRect.left + textRect.width / 2.0f, 
                       textRect.top  + textRect.height / 2.0f);
}

// -------------------------------------------------------------------------------------------------

void GameTable::setCardPositions() {

    this->xMid = -(globalData->screenWidth / 2.0);
    this->yMid = -(globalData->screenHeight / 2.0);

    cardPositions = { 
        {xMid + 200.0, yMid + 170.0}, // Player 1
        {xMid +  50.0, yMid + 170.0}, // Player 2
        {xMid - 100.0, yMid + 170.0}, // Player 3
        {xMid + 200.0, yMid - 25.0},  // Player 4
        {xMid +  50.0, yMid - 25.0},  // Player 5
        {xMid - 100.0, yMid - 25.0}   // Player 6
    };

    // Apply screen positions to player card decks
    for(short i = 0; i < numberOfPlayers; i++) {
        for(short j = 0; j < playerList[i]->hand.size(); j++) {
            playerList[i]->hand[j]->cardSprite.setOrigin(cardPositions[i].first, cardPositions[i].second);
        }
    }

    cardDeck.cardBacks[0].setOrigin(xMid + 200.f, yMid + 340.f);              
    cardDeck.cardBacks[1].setOrigin(xMid +  50.f, yMid + 340.f);   
    cardDeck.cardBacks[2].setOrigin(xMid - 100.f, yMid + 340.f);   
    cardDeck.cardBacks[3].setOrigin(xMid + 200.f, yMid - 195.f);
    cardDeck.cardBacks[4].setOrigin(xMid +  50.f, yMid - 195.f);
    cardDeck.cardBacks[5].setOrigin(xMid - 100.f, yMid - 195.f);
}

// -------------------------------------------------------------------------------------------------

void GameTable::setGameSpeed(short speed) {

    // Speeds (slow to fast)     0    1    2    3     4     5     6     7     8     9     10
    short placementDelay[]  = {  3,   6,  12,  25,   50,  100,  150,  200,  250,  350,   525};
    short resultsDelay[]    = { 21,  42,  83, 166,  333,  666, 1000, 1333, 1666, 2332,  3500};
    short conclusionDelay[] = { 62, 125, 250, 500, 1000, 2000, 3000, 4000, 5000, 7000, 10500};
    short tieDelay[]        = { 36,  78, 156, 312,  625, 1250, 2500, 3500, 4500, 6300,  9500};

    this->placementDelay  = placementDelay[speed];
    this->resultsDelay    = resultsDelay[speed];
    this->conclusionDelay = conclusionDelay[speed];
    this->tieDelay        = tieDelay[speed];
}

// -------------------------------------------------------------------------------------------------

void GameTable::setGreenRectanglePositions() {
    for(short i = 0; i < numberOfPlayers; i++) {
        jc::Sprite sprite;
        sprite.setTextureRect(sf::IntRect(0, 0, 80, 124));
        sprite.setTexture(globalData->textures.textures["cardPositionMarker"]);
        sprite.setOrigin(cardPositions[i].first - 10, cardPositions[i].second - 10);
        greenRectangles.push_back(sprite);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::setFontFamily() {
    if (!this->font.loadFromFile(this->fontFile)) {
        cout << "ERROR: GameTable::setText(): Font " << fontFile << " not found." << endl; 
        exit(139);
    }   
}

// -------------------------------------------------------------------------------------------------

void GameTable::setAndPlaceDeckNumberText() {

    vector<pair<float, float>> textPositions = {
        {100.f, 108.f}, // Player 1
        {250.f, 108.f}, // Player 2
        {400.f, 108.f}, // Player 3
        {100.f, 643.f}, // Player 4
        {250.f, 643.f}, // Player 5
        {400.f, 643.f}  // Player 6
    };

    for(short i = 0; i < numberOfPlayers; i++) {
        handSizeNumbers.push_back(jc::Text());
        handSizeNumbers[i].setFont(font); 
        handSizeNumbers[i].setCharacterSize(50); 
        handSizeNumbers[i].setFillColor(sf::Color(255, 255, 255));
        handSizeNumbers[i].setString(to_string(playerList[i]->numCardsInHand));
        centerTextAlignment(handSizeNumbers[i]);
        handSizeNumbers[i].setPosition(sf::Vector2f(textPositions[i].first, textPositions[i].second));     
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::setAndPlaceVictoryText() {

    for(short i = 0; i < numberOfPlayers; i++) {
        winnerText.push_back(jc::Text());
        winnerText[i].setFont(font); 
        winnerText[i].setCharacterSize(70); 
        winnerText[i].setFillColor(sf::Color(60, 210, 60));
        winnerText[i].setString("Winner");

        tieText.push_back(jc::Text());
        tieText[i].setFont(font);
        tieText[i].setCharacterSize(70);
        tieText[i].setFillColor(sf::Color::Cyan);
        tieText[i].setString("Tie");

        centerTextAlignment(winnerText[i]);
        centerTextAlignment(tieText[i]);

        winnerText[i].setPosition(sf::Vector2f(-cardPositions[i].first  + 50.f, 
                                               -cardPositions[i].second + 72.f));  

        tieText[i].setPosition(sf::Vector2f(-cardPositions[i].first  + 50.f, 
                                            -cardPositions[i].second + 72.f));   
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::verifyNumberOfPlayers() {
    if(numberOfPlayers < 2 || numberOfPlayers > 6) {
        cout << "ERROR: GameTable::numberOfPlayers: Out of range. Value must be 2-6." << endl;
        exit(139);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::generatePlayers() {
    for(short i = 0; i < numberOfPlayers; i++) {
        playerList.push_back(make_shared<Player>());
        playerList[i]->number = i + 1;
        playerList[i]->name = "Player " + to_string(i + 1);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::dealCardsToPlayers() {
    vector<vector<shared_ptr<Card>>> hands;
    hands = cardDeck.divideDeck(numberOfPlayers);
    for(short i = 0; i < numberOfPlayers; i++) {
        playerList[i]->hand = hands[i];
        playerList[i]->numCardsInHand = hands[i].size();
        playerList[i]->topCard = playerList[i]->hand[0];

        if(mayForceTie) forceTie(playerList[i]); 
    }

    if(testLastCardTie) playerList[0]->hand = playerList[1]->hand;
    if(testGameEnding) {
        playerList[0]->hand = {};
        for(auto i : playerList[0]->hand) {
            if(i->value == 2)
                playerList[1]->hand.push_back(i);
        }
    }

}

// =================================================================================================
//                                          Game Mechanics
// =================================================================================================

void GameTable::activateGameRound() {
    timeElapsed = clock.getElapsedTime().asMilliseconds();

    if(mayStartNewRound) revealFaceUpCardsWithDelayOf(placementDelay);
    if(allPlayersPlayedACard && hiPlayersNotYetDetermined) determineWinnerOrTie();
    if(mayDeclareRoundResults && timeElapsed > resultsDelay) declareRoundResults();
    if(mayBreakTie) breakTie();
    if(mayConcludeRound && timeElapsed > conclusionDelay) concludeRound();
}

// -------------------------------------------------------------------------------------------------

void GameTable::concludeRound() {
    mayConcludeRound = false;
    mayDisplayWinnerText = false;
    gatherPlayedCards();
    awardPrizePotToVictor();
    clearFaceUpCards();
    resetRoundVariables();
    removeDefeatedPlayers();
    checkForGameWinner();
}

// -------------------------------------------------------------------------------------------------

void GameTable::checkForGameWinner() {
    short numPlayersStillInGame;
    for(auto i : playerList) {
        if(!i->isOutOfGame) 
            numPlayersStillInGame++;
    }
    if(numPlayersStillInGame == 1)
        gameOver = true;

}

// -------------------------------------------------------------------------------------------------

void GameTable::removeDefeatedPlayers() {
    vector<short> activePlayersIndecies = getActivePlayerIndecies();
    for(short i = 0; i < activePlayersIndecies.size(); i++) {
        shared_ptr<Player> player = playerList[activePlayersIndecies[i]];
        if(player->numCardsInHand < 1 && !player->isOutOfGame) {
            player->isOutOfGame = true;
        }
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::gatherPlayedCards() {
    short cardsPlayed = 0;
    short totalCards = 0;

    for(short i = 0; i < numberOfPlayers; i++) {
        if(playerList[i]->isOutOfGame)
            continue;
        
        cardsPlayed = playerList[i]->hand.size() - playerList[i]->numCardsInHand;
        for(int j = 0; j < cardsPlayed; j++) {
            prizePot.push_back(playerList[i]->hand[0]);
            playerList[i]->hand.erase(playerList[i]->hand.begin());
        }
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::awardPrizePotToVictor() {
    shared_ptr<Player> winner = winnerPool[0];
    for(auto & i : prizePot) {
        i->cardSprite.setOrigin(cardPositions[winner->number - 1].first, cardPositions[winner->number - 1].second);
        winner->hand.push_back(i);
    }
    winner->numCardsInHand = winner->hand.size();
    handSizeNumbers[winner->number - 1].setString(to_string(winner->numCardsInHand));
    centerTextAlignment(handSizeNumbers[winner->number - 1]);
    prizePot = {};
}

// -------------------------------------------------------------------------------------------------

void GameTable::clearFaceUpCards() {
    allPlayersPlayedACard = false;
    faceupCards = 0;
}

// -------------------------------------------------------------------------------------------------

void GameTable::resetRoundVariables() {

    // Set player cards to display first card
    for(short i = 0; i < numberOfPlayers; i++) {
        if(playerList[i]->isOutOfGame)
            continue;

        playerList[i]->topCard = playerList[i]->hand[0];
    }

    mayClickOnCard = true;
    tieBreakerIndex = 1;
    tieRound = 0;

    scanForDuplicateCards();
    // verifyCardsEqualDeck();
}

// -------------------------------------------------------------------------------------------------

void GameTable::determineWinnerOrTie() {
    if(isATie)
        fillWinnerPool(winnerPool, tieBreakerIndex++); 
    else
        fillWinnerPool(playerList, 0); 

    if(winnerPool.size() > 1) {
        isATie = true;
        tieRound++;
    }
    else
        isATie = false;

    hiPlayersNotYetDetermined = false;
    mayDeclareRoundResults = true;
    clock.restart();
}

// -------------------------------------------------------------------------------------------------

void GameTable::declareRoundResults() {
    if(isATie) {
        globalData->gameSound.playSoundEffect("tie.ogg");
        mayDisplayTieText = true;
        mayBreakTie = true;
    }
    else {
        globalData->gameSound.playSoundEffect("winner2.ogg");
        mayDisplayWinnerText = true;
        mayConcludeRound = true;
    }

    mayDeclareRoundResults = false;
    clock.restart();
}

// -------------------------------------------------------------------------------------------------

void GameTable::breakTie() {
    // 1 or more players tie on their last card
    for(auto i : winnerPool) {
        if(!i->hand[tieRound]) { // There is no card after the current card played
            breakTieOnLastCard();
            return;
        }
    }

    static short delay = tieDelay;
    if(timeElapsed > delay) {
        mayDisplayTieText = false;
        static short nextWinnerIndex = 0;
        playTieBreakerCard(winnerPool[nextWinnerIndex++]);
        delay = tieDelay / 10;

        if(nextWinnerIndex >= winnerPool.size()) {
            nextWinnerIndex = 0;
            delay = tieDelay;
            mayBreakTie = false;
            hiPlayersNotYetDetermined = true;
        }

    clock.restart();
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::breakTieOnLastCard() {
    cout << "breakTieOnLastCard() called because someone tied on their last card." << endl;
    // 2-way tie - left player is out
    // 2-way tie - right player is out
    // 2-way tie - both players are out
    // 3-way tie - 1 or more players are out
    // #-way tie - all players are out.

}

// -------------------------------------------------------------------------------------------------

void GameTable::playTieBreakerCard(shared_ptr<Player> player) {
    adjustHandSizeNumber(player);
    short ith_card = tieRound;
    player->topCard = player->hand[ith_card]; 
    // printPlayerMove(player);
}

// -------------------------------------------------------------------------------------------------

void GameTable::fillWinnerPool(vector<shared_ptr<Player>> competingPlayers, short ith_card) {
    winnerPool = {};
    short highestCard = getHighestCardValuePlayed(competingPlayers, ith_card);

    for(short i = 0; i < competingPlayers.size(); i++) {
        if(competingPlayers[i]->isOutOfGame)
            continue;

        short currentCard = competingPlayers[i]->hand[ith_card]->value;
        if(currentCard == highestCard) {
            winnerPool.push_back(competingPlayers[i]);
        }
    }    
}

// -------------------------------------------------------------------------------------------------

short GameTable::getHighestCardValuePlayed(vector<shared_ptr<Player>> competingPlayers, short ith_card) {
    short highestCard = 0;

    for(short i = 0; i < competingPlayers.size(); i++) {
        if(competingPlayers[i]->isOutOfGame)
            continue;

        short currentCard = competingPlayers[i]->hand[ith_card]->value;
        if(currentCard > highestCard)
            highestCard = currentCard;
    }

    return highestCard;
}

// -------------------------------------------------------------------------------------------------

void GameTable::revealFaceUpCardsWithDelayOf(short delay) {
    vector<short> activePlayerIndecies = getActivePlayerIndecies();

    // Stops this function from repeating after x loops.
    if(faceupCards >= activePlayerIndecies.size()) {
        hiPlayersNotYetDetermined = true;
        mayStartNewRound = false;
        allPlayersPlayedACard = true;
    }

    // Do the animation of placing cards as long as less than x cards have been placed.
    if(elapsed.asMilliseconds() > delay && faceupCards < activePlayerIndecies.size()) {
        faceupCards++;

        shared_ptr<Player> currentPlayer = playerList[activePlayerIndecies[faceupCards - 1]];

        clock.restart();
        adjustHandSizeNumber(currentPlayer);
        // printPlayerMove(currentPlayer);

        // For debugging
        // cout << "Faceup cards: " << faceupCards << " "
        //      << "activePlayerIndecies: (#" << activePlayerIndecies.size() << ") {";
        // for(auto i : activePlayerIndecies)
        //     cout << i << ", ";
        // cout << "}" << endl;
    }
}

// -------------------------------------------------------------------------------------------------

// Adjusts card deck number and plays sound
void GameTable::adjustHandSizeNumber(shared_ptr<Player> player) {
    player->numCardsInHand--;
    handSizeNumbers[player->number - 1].setString(to_string(player->numCardsInHand));
    if(player->numCardsInHand < 1) 
        handSizeNumbers[player->number - 1].setString("Out");
    centerTextAlignment(handSizeNumbers[player->number - 1]);

    short randNum = Miscellaneous::generateRandomNumber(2);
    globalData->gameSound.playSoundEffect("cardDeal" + to_string(randNum) + ".ogg");
}

// -------------------------------------------------------------------------------------------------
// Loops always
void GameTable::drawPlayerFaceUpCards() {
    vector<short> activePlayerIndecies = getActivePlayerIndecies();
    for(short i = 0; i < faceupCards; i++) {
        if(i < activePlayerIndecies.size()) 
            globalData->window.draw(playerList[activePlayerIndecies[i]]->topCard->cardSprite);
    }
}

// -------------------------------------------------------------------------------------------------

vector<short> GameTable::getActivePlayerIndecies() {
    vector<short> activePlayerIndecies = {};
    for(auto i : playerList)
        if(!i->isOutOfGame)
            activePlayerIndecies.push_back(i->number - 1);

    return activePlayerIndecies;
}

// -------------------------------------------------------------------------------------------------

void GameTable::drawCardsBacksAndNumbers() {
    for(short i = 0; i < numberOfPlayers; i++) {
        vector<short> activePlayerIndecies = getActivePlayerIndecies();
        globalData->window.draw(greenRectangles[i]);
        if(playerList[i]->numCardsInHand > 0)
            globalData->window.draw(cardDeck.cardBacks[i]);
        globalData->window.draw(handSizeNumbers[i]);
        elapsed = clock.getElapsedTime();
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::runGameLoop() {
    while(globalData->window.isOpen()) {
        globalData->eventHandler.listen();
        eventMonitor();
        globalData->window.clear(sf::Color(0, 90, 0));

        if(!gameOver) 
            activateGameRound();
        else {
            vector<short> winner = getActivePlayerIndecies();
            handSizeNumbers[winner[0]].setString("  Winner!\nGame Over");
            handSizeNumbers[winner[0]].setFillColor(sf::Color::Blue);
            centerTextAlignment(handSizeNumbers[winner[0]]);
        }

        drawAllTableSprites();
        globalData->window.display();

    } 
}   

// -------------------------------------------------------------------------------------------------

void GameTable::drawAllTableSprites() {
    drawCardsBacksAndNumbers();
    drawPlayerFaceUpCards();

    if(mayDisplayWinnerText) globalData->window.draw(winnerText[winnerPool[0]->number - 1]);

    if(mayDisplayTieText) {
        for(short i = 0; i < winnerPool.size(); i++) {
            globalData->window.draw(tieText[winnerPool[i]->number - 1]);
        }
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::checkForMouseClickOnCard() {
    if(globalData->eventHandler.cardWasClicked || autoClick) {
        mayStartNewRound = true;
        mayClickOnCard = false;
        globalData->eventHandler.cardWasClicked = false;
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::eventMonitor() {
    if(mayClickOnCard) checkForMouseClickOnCard();
    // Future events
}

// -------------------------------------------------------------------------------------------------

void GameTable::printAllPlayerStats() {
    for(short i = 0; i < numberOfPlayers; i++) {
        cout << "\n==============================\n\tPlayer " << i + 1 << "\n\tHand size: " 
                << playerList[i]->hand.size() << "\n\tCards on deck: " << playerList[i]->numCardsInHand 
                << "\n\tTop card: " << playerList[i]->topCard->cardName 
                << "\n==============================\n";
        for(short j = 0; j < playerList[i]->hand.size(); j++) {
            cout << playerList[i]->hand[j]->cardName << "\t{" 
                 << playerList[i]->hand[j]->cardSprite.getOrigin().x << ", "
                 << playerList[i]->hand[j]->cardSprite.getOrigin().y << "}\n";
        }
    }
    cout << "========================================================\n";
}

// -------------------------------------------------------------------------------------------------

void GameTable::printAllBooleanPermissions() {
    cout << "============= Booleans =============" << endl;
    cout << "mayStartNewRound: "          << mayStartNewRound          << endl;
    cout << "mayClickOnCard: "            << mayClickOnCard            << endl;
    cout << "allPlayersPlayedACard: "     << allPlayersPlayedACard     << endl;    
    cout << "mayDeclareRoundResults: "    << mayDeclareRoundResults    << endl;   
    cout << "hiPlayersNotYetDetermined: " << hiPlayersNotYetDetermined << endl; 
    cout << "mayDisplayWinnerText: "      << mayDisplayWinnerText      << endl;
    cout << "mayDisplayTieText: "         << mayDisplayTieText         << endl;
    cout << "isATie: "                    << isATie                    << endl;
    cout << "mayForceTie: "               << mayForceTie               << endl;
    cout << "mayBreakTie: "               << mayBreakTie               << endl;
    cout << "mayConcludeRound: "          << mayConcludeRound          << endl;
}

// -------------------------------------------------------------------------------------------------

void GameTable::printPrizePotContents() {
    cout << "=============== Prize Pot: ===============" << endl;
    for(auto i : prizePot) {
        cout << i->cardName << "\t" << i->cardSprite.getOrigin().x << ", " << i->cardSprite.getOrigin().y << endl;
    }
    cout << "Prize Pot size: " << prizePot.size() << endl;
}

// -------------------------------------------------------------------------------------------------

void GameTable::scanForDuplicateCards() {
    set<string> deckChecker;
    short beforeSize, afterSize;
    for(short i = 0; i < numberOfPlayers; i++) {
        for(short j = 0; j < playerList[i]->hand.size(); j++) {
            beforeSize = deckChecker.size();
            deckChecker.insert(playerList[i]->hand[j]->cardName);
            afterSize = deckChecker.size();
            if(beforeSize == afterSize) {
                cout << "ERROR: Duplicate cards detected starting with " << playerList[i]->hand[j]->cardName << endl;
                exit(139);
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------
// The player will still be part of the game, but considered out of cards (for debugging).
void GameTable::kickPlayer(short playerNumber) {
    playerList[playerNumber - 1]->isOutOfGame = true;
    handSizeNumbers[playerNumber - 1].setString("Kicked");
    centerTextAlignment(handSizeNumbers[playerNumber - 1]);
}

// -------------------------------------------------------------------------------------------------

void GameTable::forceTie(shared_ptr<Player> player) {
    shared_ptr<Card> temp = NULL;
    for(short i = 0; i < player->hand.size(); i++) {
        // Swap ace with top card
        if(player->hand[i]->value == 14) {
            temp = player->hand[0];
            player->hand[0] = player->hand[i];
            player->hand[i] = temp;
            player->topCard = player->hand[0];
        }
    }
    // Double tie - swap king with 2nd from top card
    for(short i = 0; i < player->hand.size(); i++) {
        if(player->hand[i]->value == 13) {
            temp = player->hand[1];
            player->hand[1] = player->hand[i];
            player->hand[i] = temp;
        }
    }            
}

// -------------------------------------------------------------------------------------------------

void GameTable::printPlayerMove(shared_ptr<Player> player) {
    cout << player->name << " is out: " << player->isOutOfGame 
         << " Cards on deck: " << player->numCardsInHand << "\t" << player->topCard->cardName << endl; 
}