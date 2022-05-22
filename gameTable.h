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

using namespace std;

class GameTable {
public:

    GameTable() {}
    GameTable(Initializer & globalData);
    void construct(Initializer & globalData);

    void gameTableLoop();

    short numberOfPlayers = 0;

    
private:

    short faceupCards = 0;
    short timeElapsed = 0;
    short tieRound = 0;
    short tieBreakerIndex = 1;
    short placementDelay = 150;
    short resultsDelay = 1000;
    short conclusionDelay = 3000;
    short tieDelay = 2500;

    string cardBack = "redCardBack";
    Initializer* globalData;
    CardDeck cardDeck; 
    float xMid, yMid;

    vector<shared_ptr<Player>> playerList;
    vector<pair<float, float>> cardPositions;
    vector<shared_ptr<Player>> winnerPool;
    vector<shared_ptr<Card>>   prizePot;
    vector<sf::RectangleShape> greenRectangles;
    vector<sf::Text>           deckSizeNumbers;
    vector<sf::Text>           winnerText;
    vector<sf::Text>           tieText;

    bool mayClick                  = true;
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
    bool buttonIsHeld              = false;

    vector<pair<float, float>> deckSizeTextPositions = {
        {100.f, 108.f}, // Player 1
        {250.f, 108.f}, // Player 2
        {400.f, 108.f}, // Player 3
        {100.f, 643.f}, // Player 4
        {250.f, 643.f}, // Player 5
        {400.f, 643.f}  // Player 6
    };

    pair<pair<float, float>, pair<float, float>> const cardClickArea = {
        {50, 150}, {37, 183}
    };

    pair<pair<float, float>, pair<float, float>> gearIconClickArea = {
        {10, 40}, {10, 40}
    };    

	sf::Font  font; 
    sf::Text  headingText;
    sf::Clock clock;
    sf::Time  elapsed; 
    sf::Sprite gearMenuIcon;

    // ---------------------------------------------------------------------------------------------

    void set_GearMenuIcon();
    void set_CardPositions();
    void set_CardBackPositions();
    void set_DeckSizeText();
    void set_GreenRectangles();
    void set_GameSpeed(short speed);
    void set_VictoryText();

    void draw_CardsBacks();
    void draw_GreenRectangles();
    void draw_DeckSizeNumbers();
    void draw_AllTableSprites();

    void listen_ForMouseClicks();
    void listener_CardWasClicked(float x, float y);
    void listener_GearMenuIconClick(float x, float y);
    void listener_MenuEventMonitor();

    bool leftClick();
    bool leftRelease();

    // ------------------------------

    void verifyNumberOfPlayers();
    void generatePlayers();
    void dealCardsToPlayers();  
    vector<short> getActivePlayerIndecies();

    void activateGameRound();
    void revealFaceUpCardsWithDelayOf(short delay);
    void drawPlayerFaceUpCards();
    void adjustDeckSizeNumber(shared_ptr<Player> player);

    short getHighestCardValuePlayed(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void fillWinnerPool(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void declareRoundResults();
    void determineWinnerOrTie();
    void breakTie();
    void breakTieOnLastCard(shared_ptr<Player> player);
    void playTieBreakerCard(shared_ptr<Player> player);

    void gatherPlayedCards();
    void popPlayedCardsFromPlayerDecks();
    void awardPrizePotToVictor();
    void concludeRound();
    void clearFaceUpCards();
    void resetRoundVariables();
    void removeDefeatedPlayers();
    void checkForGameWinner();

    void printAllPlayerStats();
    void printAllBooleanPermissions();
    void printPrizePotContents();
    void printPlayerMove(shared_ptr<Player> player);
    void changeCardStyle();
};


// =================================================================================================


GameTable::GameTable(Initializer & globalData) {
    construct(globalData);
}

// -------------------------------------------------------------------------------------------------

void GameTable::construct(Initializer & globalData) {
    this->cardDeck = CardDeck(globalData);
    this->globalData = &globalData;
    this->numberOfPlayers = globalData.numberOfPlayers;
    this->font = globalData.defaultFont;        
    verifyNumberOfPlayers();
    generatePlayers();
    dealCardsToPlayers();
    set_GearMenuIcon();
    set_GameSpeed(globalData.gameSpeed);
    set_CardPositions();
    set_CardBackPositions();
    set_VictoryText();
    set_GreenRectangles();
    set_DeckSizeText();
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_GearMenuIcon() {
	gearMenuIcon.setTextureRect(sf::IntRect(0, 0, 30, 30));
    gearMenuIcon.setTexture(globalData->textures.textures["gearMenuIcon"]);
    gearMenuIcon.setOrigin(-10, -10);
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_CardPositions() {

    xMid = -(globalData->screenWidth / 2.0);
    yMid = -(globalData->screenHeight / 2.0);

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
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_CardBackPositions() {
    cardDeck.cardBacks[0].setOrigin(xMid + 200.f, yMid + 340.f);              
    cardDeck.cardBacks[1].setOrigin(xMid +  50.f, yMid + 340.f);   
    cardDeck.cardBacks[2].setOrigin(xMid - 100.f, yMid + 340.f);   
    cardDeck.cardBacks[3].setOrigin(xMid + 200.f, yMid - 195.f);
    cardDeck.cardBacks[4].setOrigin(xMid +  50.f, yMid - 195.f);
    cardDeck.cardBacks[5].setOrigin(xMid - 100.f, yMid - 195.f);
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_GameSpeed(short speed) {

    // Reverse speed variable so 1 is fastest and 10 slowest.
    short reverse[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    speed = reverse[speed];

    // Speeds (slow to fast)     1    2     3     4     5     6     7     8     9    10
    short placementDelay[]  = { 36,  47,   63,   84,  113,  150,  195,  260,  338,  439};
    short resultsDelay[]    = {237, 316,  422,  563,  750, 1000, 1300, 1690, 2197, 2856};
    short conclusionDelay[] = {712, 949, 1266, 1688, 2250, 3000, 3900, 5070, 6591, 8568};
    short tieDelay[]        = {474, 632,  843, 1125, 1500, 2000, 2600, 3380, 4394, 5712};

    this->placementDelay  = placementDelay[speed];
    this->resultsDelay    = resultsDelay[speed];
    this->conclusionDelay = conclusionDelay[speed];
    this->tieDelay        = tieDelay[speed];
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_GreenRectangles() {
    for(short i = 0; i < numberOfPlayers; i++) {
        sf::RectangleShape greenRectangle;
        greenRectangle.setSize(sf::Vector2f(60.f, 93.f));
        greenRectangle.setFillColor(sf::Color(0.f, 90.f, 0.f));
        greenRectangle.setOutlineColor(sf::Color(100.f, 150.f, 100.f));
        greenRectangle.setOutlineThickness(8.f);
        greenRectangle.setOrigin(cardPositions[i].first - 20,  cardPositions[i].second - 25);
        greenRectangles.push_back(greenRectangle);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_DeckSizeText() {
    for(short i = 0; i < numberOfPlayers; i++) {
        deckSizeNumbers.push_back(sf::Text());
        deckSizeNumbers[i].setFont(font); 
        deckSizeNumbers[i].setCharacterSize(50); 
        deckSizeNumbers[i].setFillColor(sf::Color(255, 255, 255));
        deckSizeNumbers[i].setString(to_string(playerList[i]->numCardsInHand));
        Miscellaneous::centerTextAlignment(deckSizeNumbers[i]);
        deckSizeNumbers[i].setPosition(sf::Vector2f(deckSizeTextPositions[i].first, 
                                                    deckSizeTextPositions[i].second));     
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::set_VictoryText() {

    for(short i = 0; i < numberOfPlayers; i++) {
        winnerText.push_back(sf::Text());
        winnerText[i].setFont(font); 
        winnerText[i].setCharacterSize(70); 
        winnerText[i].setFillColor(sf::Color(60, 210, 60));
        winnerText[i].setString("Winner");

        tieText.push_back(sf::Text());
        tieText[i].setFont(font);
        tieText[i].setCharacterSize(70);
        tieText[i].setFillColor(sf::Color::Cyan);
        tieText[i].setString("Tie");

        Miscellaneous::centerTextAlignment(winnerText[i]);
        Miscellaneous::centerTextAlignment(tieText[i]);

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
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::listen_ForMouseClicks() {

    float mouseX = sf::Mouse::getPosition(globalData->window).x;
    float mouseY = sf::Mouse::getPosition(globalData->window).y;

    if(leftClick()) {
        listener_GearMenuIconClick (mouseX, mouseY);
        listener_CardWasClicked    (mouseX, mouseY);
    }

    if(leftRelease()) {
    }

    if(globalData->autoClick && mayClick) {
        mayStartNewRound = true;
        mayClick = false;
    }
}

// -------------------------------------------------------------------------------------------------

bool GameTable::leftClick() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonIsHeld) {
        buttonIsHeld = true;
        return true;
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

bool GameTable::leftRelease() {
    if(globalData->eventHandler.mouseRelease) {
        buttonIsHeld = false;
        globalData->eventHandler.mouseRelease = false;
        return true;
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

void GameTable::listener_CardWasClicked(float x, float y) {
    bool xBegin = x > cardClickArea.first.first;
    bool xEnd   = x < cardClickArea.first.second;
    bool yBegin = y > cardClickArea.second.first;
    bool yEnd   = y < cardClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        mayStartNewRound = true;
        globalData->gameSound.playSoundEffect("tClick.ogg");
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::listener_GearMenuIconClick(float x, float y) {

    bool xBegin = x > gearIconClickArea.first.first;
    bool xEnd   = x < gearIconClickArea.first.second;
    bool yBegin = y > gearIconClickArea.second.first;
    bool yEnd   = y < gearIconClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        globalData->gameSound.playSoundEffect("tClick.ogg");
        globalData->gameMenuIsOpen = true;
    }
}

// =================================================================================================
// ======================================== Game Mechanics =========================================
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
    deckSizeNumbers[winner->number - 1].setString(to_string(winner->numCardsInHand));
    Miscellaneous::centerTextAlignment(deckSizeNumbers[winner->number - 1]);
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

    mayClick = true;
    tieBreakerIndex = 1;
    tieRound = 0;
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
        globalData->gameSound.playSoundEffect("winner.ogg");
        mayDisplayWinnerText = true;
        mayConcludeRound = true;
    }

    mayDeclareRoundResults = false;
    clock.restart();
}

// -------------------------------------------------------------------------------------------------

void GameTable::breakTie() {

    // Called when a player ties on their last card and cannot break it with another card.
    for(short i = 0; i < winnerPool.size(); i++) {
        if(!winnerPool[i]->numCardsInHand) { 
            winnerPool.erase(winnerPool.begin() + i);

            if(winnerPool.size() == 1) {
                mayDisplayTieText = false;
                mayDeclareRoundResults = true;
                isATie = false;
                mayBreakTie = false;
                return;
            }
        }
    }

    // Default tie breaker. Iterates tie player's card to next in hand, then send back through loop.
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

void GameTable::breakTieOnLastCard(shared_ptr<Player> player) {
    cout << player->name << " played their last card and it was a tie! (Please don't crash!)" << endl;
    printAllPlayerStats();

    // Strip player from winnerPool
    for(short i = 0; i < winnerPool.size(); i++) {
        if(winnerPool[i]->number = player->number)
            winnerPool.erase(winnerPool.begin() + i);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::playTieBreakerCard(shared_ptr<Player> player) {
    adjustDeckSizeNumber(player);
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
        adjustDeckSizeNumber(currentPlayer);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::adjustDeckSizeNumber(shared_ptr<Player> player) {
    player->numCardsInHand--;
    deckSizeNumbers[player->number - 1].setString(to_string(player->numCardsInHand));
    if(player->numCardsInHand < 1) 
        deckSizeNumbers[player->number - 1].setString("Out");
    Miscellaneous::centerTextAlignment(deckSizeNumbers[player->number - 1]);

    short randNum = Miscellaneous::generateRandomNumber(2);
    globalData->gameSound.playSoundEffect("cardDeal" + to_string(randNum) + ".ogg");
}

// -------------------------------------------------------------------------------------------------

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

void GameTable::draw_CardsBacks() {
    for(short i = 0; i < numberOfPlayers; i++) {

        vector<short> activePlayerIndecies = getActivePlayerIndecies();
        
        if(playerList[i]->numCardsInHand > 0)
            globalData->window.draw(cardDeck.cardBacks[i]);

        elapsed = clock.getElapsedTime();
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::draw_GreenRectangles() {
    for(short i = 0; i < greenRectangles.size(); i++) {
        globalData->window.draw(greenRectangles[i]);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::draw_DeckSizeNumbers() {
    for(short i = 0; i < deckSizeNumbers.size(); i++) {
        globalData->window.draw(deckSizeNumbers[i]);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::gameTableLoop() {
        if(!gameOver) 
            activateGameRound();
        else {
            vector<short> winner = getActivePlayerIndecies();
            deckSizeNumbers[winner[0]].setString("  Winner!\nGame Over");
            deckSizeNumbers[winner[0]].setFillColor(sf::Color::Blue);
            Miscellaneous::centerTextAlignment(deckSizeNumbers[winner[0]]);
        }

        draw_AllTableSprites();
        listen_ForMouseClicks();
        listener_MenuEventMonitor();
}

// -------------------------------------------------------------------------------------------------

void GameTable::draw_AllTableSprites() {
    draw_GreenRectangles();
    draw_CardsBacks();
    draw_DeckSizeNumbers();
    drawPlayerFaceUpCards();
    globalData->window.draw(gearMenuIcon);

    if(mayDisplayWinnerText) globalData->window.draw(winnerText[winnerPool[0]->number - 1]);

    if(mayDisplayTieText) {
        for(short i = 0; i < winnerPool.size(); i++) {
            globalData->window.draw(tieText[winnerPool[i]->number - 1]);
        }
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::listener_MenuEventMonitor() {
    set_GameSpeed(globalData->gameSpeed);
    changeCardStyle();
}

// -------------------------------------------------------------------------------------------------

void GameTable::changeCardStyle() {
    if(cardBack != globalData->cardBack) {
        cardBack = globalData->cardBack;
        cardDeck.generateCardBacks(numberOfPlayers);
        set_CardBackPositions();
    }
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
    cout << "mayClick: "                  << mayClick                  << endl;
    cout << "allPlayersPlayedACard: "     << allPlayersPlayedACard     << endl;    
    cout << "mayDeclareRoundResults: "    << mayDeclareRoundResults    << endl;   
    cout << "hiPlayersNotYetDetermined: " << hiPlayersNotYetDetermined << endl; 
    cout << "mayDisplayWinnerText: "      << mayDisplayWinnerText      << endl;
    cout << "mayDisplayTieText: "         << mayDisplayTieText         << endl;
    cout << "isATie: "                    << isATie                    << endl;
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

void GameTable::printPlayerMove(shared_ptr<Player> player) {
    cout << player->name << " is out: " << player->isOutOfGame 
         << " Cards on deck: " << player->numCardsInHand << "\t" << player->topCard->cardName << endl; 
}

#endif // GAMETABLE_H