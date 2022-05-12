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

private:

    short numberOfPlayers = 6;
    short faceupCards = 0;
    short timeElapsed = 0;
    short tieRound = 0;
    short tieBreakerIndex = 1;

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
    bool forceTie                  = false;
    bool mayBreakTie               = false;
    bool mayConcludeRound          = false;
    
	sf::Font  font; 
    jc::Text  headingText;
    sf::Clock clock;
    sf::Time  elapsed; 

    // ---------------------------------------------------------------

    void setFontFamily();
    void setHeadingText();
    void setAndPlaceVictoryText();
    void setCardPositions();
    void setGreenRectanglePositions();
    void setAndPlaceDeckNumberText();
    void setAndPlaceTieText();
    void centerTextAlignment(jc::Text & someText);

    void verifyNumberOfPlayers();
    void generatePlayers();
    void dealCardsToPlayers();    
    void drawCardsBacksAndNumbers();
    void drawAllTableSprites();

    void activateGameRound();
    void revealFaceUpCardsWithDelayOf(short delay);
    void drawPlayerFaceUpCards();
    void playNextCardOnDeck(shared_ptr<Player> player);

    short getHighestCardValuePlayed(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void fillWinnerPool(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void declareRoundResults();
    void determineWinnerOrTie();
    void breakTie();
    void playTieBreakerCard(shared_ptr<Player> player);

    void gatherPlayedCards();
    void popPlayedCardsFromPlayerDecks();
    void awardPrizePotToVictor();
    void concludeRound();
    void clearFaceUpCards();
    void resetRoundVariables();
    void removeDefeatedPlayers();

    void eventMonitor();
    void checkForMouseClickOnCard();
    void printAllPlayerStats();
    void printAllBooleanPermissions();
    void printPrizePotContents();
    void scanForDuplicateCards();
    void verifyCardsEqualDeck();
};

#endif // GAMETABLE_H


// =================================================================================================


GameTable::GameTable(Initializer & globalData) : cardDeck(globalData) {
    this->globalData = &globalData;
    verifyNumberOfPlayers();
    generatePlayers();
    dealCardsToPlayers();
    setFontFamily();
    setCardPositions();
    setAndPlaceVictoryText();
    setGreenRectanglePositions();
    setAndPlaceDeckNumberText();
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

    // for(short i = 0, j = -100, k = -50; i < cardDeck.deck.size(); i++, j -= 40, k -= 10) {
    //     cardDeck.deck[i].cardSprite.setOrigin(j, k);
    //     if(i == 25) {
    //         j = -60;
    //     }
    // }

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

        if(forceTie) {
            shared_ptr<Card> temp = NULL;
            for(short j = 0; j < playerList[i]->hand.size(); j++) {
                // Swap ace with top card
                if(playerList[i]->hand[j]->value == 14) {
                    temp = playerList[i]->hand[0];
                    playerList[i]->hand[0] = playerList[i]->hand[j];
                    playerList[i]->hand[j] = temp;
                    playerList[i]->topCard = playerList[i]->hand[0];
                }
            }
            // Double tie - swap king with 2nd from top card
            for(short j = 0; j < playerList[i]->hand.size(); j++) {
                if(playerList[i]->hand[j]->value == 13) {
                    temp = playerList[i]->hand[1];
                    playerList[i]->hand[1] = playerList[i]->hand[j];
                    playerList[i]->hand[j] = temp;
                }
            }            
        }
    }
}

// =================================================================================================
//                                          Game Mechanics
// =================================================================================================

void GameTable::activateGameRound() {
    timeElapsed = clock.getElapsedTime().asMilliseconds();

    if(mayStartNewRound) revealFaceUpCardsWithDelayOf(150);
    if(allPlayersPlayedACard && hiPlayersNotYetDetermined) determineWinnerOrTie();
    if(mayDeclareRoundResults && timeElapsed > 1000) declareRoundResults();
    if(mayBreakTie) breakTie();
    if(mayConcludeRound && timeElapsed > 3000) concludeRound();
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
}

// -------------------------------------------------------------------------------------------------

void GameTable::removeDefeatedPlayers() {
    for(auto i : playerList) {
        if(i->numCardsInHand < 1)
            i->isOutOfGame = true;
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
    static short delay = 2500;

    if(timeElapsed > delay) {
        mayDisplayTieText = false;
        static short nextWinnerIndex = 0;
        playTieBreakerCard(winnerPool[nextWinnerIndex++]);
        delay = 250;

        if(nextWinnerIndex >= winnerPool.size()) {
            nextWinnerIndex = 0;
            delay = 2500;
            mayBreakTie = false;
            hiPlayersNotYetDetermined = true;
        }

    clock.restart();
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::playTieBreakerCard(shared_ptr<Player> player) {
    playNextCardOnDeck(player);
    short ith_card = tieRound;
    player->topCard = player->hand[ith_card]; 
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
    if(faceupCards >= playerList.size()) {
        hiPlayersNotYetDetermined = true;
        mayStartNewRound = false;
        allPlayersPlayedACard = true;
    }

    if(elapsed.asMilliseconds() > 150 && faceupCards < playerList.size()) {
        faceupCards++;
        shared_ptr<Player> currentPlayer = playerList[faceupCards - 1];
        if(!currentPlayer->isOutOfGame) {
            clock.restart();
            playNextCardOnDeck(playerList[faceupCards - 1]);
        }
    }
}


// -------------------------------------------------------------------------------------------------

void GameTable::playNextCardOnDeck(shared_ptr<Player> player) {
    player->numCardsInHand--;
    handSizeNumbers[player->number - 1].setString(to_string(player->numCardsInHand));
    if(player->numCardsInHand < 1) 
        handSizeNumbers[player->number - 1].setString("Out");
    centerTextAlignment(handSizeNumbers[player->number - 1]);
    short randNum = Miscellaneous::generateRandomNumber(2);
    globalData->gameSound.playSoundEffect("cardDeal" + to_string(randNum) + ".ogg");
}

// -------------------------------------------------------------------------------------------------

void GameTable::drawPlayerFaceUpCards() {
    for(short i = 0; i < numberOfPlayers; i++) {
        if(i < faceupCards) 
            globalData->window.draw(playerList[i]->topCard->cardSprite);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::drawCardsBacksAndNumbers() {
    for(short i = 0; i < numberOfPlayers; i++) {
        globalData->window.draw(greenRectangles[i]);
        if(playerList[i]->numCardsInHand)
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

        activateGameRound();

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
    if(globalData->eventHandler.cardWasClicked) {
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
    cout << "forceTie: "                  << forceTie                  << endl;
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

void GameTable::verifyCardsEqualDeck() {
    vector<shared_ptr<Card>> completeList = {};
    for(short i = 0; i < numberOfPlayers; i++) {
        for(short j = 0; j < playerList[i]->hand.size(); j++) {
            completeList.push_back(playerList[i]->hand[j]);
            sort(completeList.begin(), completeList.end());
        }
    }

    for(auto i : completeList) {
        cout << i->cardName << endl;
    }

    cout << "Complete List size: " << completeList.size() << endl;
    // sort(cardDeck.deck.begin(), cardDeck.deck.end());
    // if(completeList != cardDeck.deck) {
    //     cout << "ERROR: Overall card deck has been altered." << endl;
    //     exit(139);
    // }
}