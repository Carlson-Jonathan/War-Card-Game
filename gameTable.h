// Copyright Jonathan Carlson 2022

#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <iostream>
#include <vector>
#include <memory>
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

    short                      numberOfPlayers = 6;
    short                      faceupCards = 0;
    short                      savedTime;

    Initializer*               globalData;
    CardDeck                   cardDeck; 

    vector<shared_ptr<Player>> playerList;
    vector<pair<float, float>> cardPositions;
    vector<pair<float, float>> roundResultPositions;
    vector<jc::Sprite>         greenRectangles;
    vector<shared_ptr<Player>> winnerPool;
    vector<jc::Text>           handSizeNumbers;
    vector<jc::Text>           winnerText;
    vector<jc::Text>           tieText;

    bool                       mayPlayNextRound       = false;
    bool                       winnerNotYetDetermined = false;
    bool                       mayDisplayWinnerText   = false;
    bool                       mayDisplayTieText      = false;
    bool                       isAWinner              = false;
    bool                       isATie                 = false;
    bool                       forceTie               = false;
    bool                       mayGetNewTime          = true;
    bool                       mayBreakTie            = false;
    
    string                     fontFile = "Fonts/Robusta-Regular.ttf";
    float                      xMid, yMid;

	sf::Font                   font; 
    jc::Text                   headingText;
    sf::Clock                  clock;
    sf::Time                   elapsed; 

    // ---------------------------------------------------------------

    void setAllTextAndSpritePositions();
    void setFontFamily();
    void setHeadingText();
    void setAndPlaceVictoryText();

    void setCardPositions();
    void setGreenRectanglePositions();
    void setAndPlaceDeckNumberText();

    void verifyNumberOfPlayers();
    void generatePlayers();
    void dealCardsToPlayers();    

    void eventMonitor();
    void checkForMouseClickOnCard();

    void activateGameRound();
    void revealFaceUpCardsWithDelayOf(short delay);
    void drawCardsBacksAndNumbers();
    void drawAllTableSprites();
    void playCardPlacementSound();

    short getHighestCardValuePlayed(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void displayTieCondition();
    void getRoundWinner(vector<shared_ptr<Player>> competingPlayers, short ith_card);
    void declareWinner();
    void setAndPlaceTieText();
    void declareTie();

    void printAllPlayerStats();


    void playNextCardOnDeck(shared_ptr<Player> player);
    void drawPlayerFaceUpCards();
    void playTieBreakerCard(shared_ptr<Player> player);
};

#endif // GAMETABLE_H


// =================================================================================================


GameTable::GameTable(Initializer & globalData) : cardDeck(globalData) {
    this->globalData = &globalData;
    verifyNumberOfPlayers();
    generatePlayers();
    dealCardsToPlayers();
    setFontFamily();
    setAllTextAndSpritePositions();
}

// -------------------------------------------------------------------------------------------------

void GameTable::setAllTextAndSpritePositions() {
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

    sf::FloatRect textRect = headingText.getLocalBounds();
    headingText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top  + textRect.height / 2.0f);
    headingText.setPosition(sf::Vector2f(globalData->screenWidth / 1.65, 60));
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
        {xMid + 200.f, yMid + 170.f}, // Player 1
        {xMid +  50.f, yMid + 170.f}, // Player 2
        {xMid - 100.f, yMid + 170.f}, // Player 3
        {xMid + 200.f, yMid - 25.f},  // Player 4
        {xMid +  50.f, yMid - 25.f},  // Player 5
        {xMid - 100.f, yMid - 25.f}   // Player 6
    };

    for(short i = 0; i < numberOfPlayers; i++) {
        for(short j = 0; j < playerList[i]->hand.size(); j++) {
            playerList[i]->hand[j]->cardSprite.setOrigin(cardPositions[i].first, cardPositions[i].second);
        }
    }

    cardDeck.cardBacks[0].setOrigin(xMid + 200, yMid + 340);              
    cardDeck.cardBacks[1].setOrigin(xMid +  50, yMid + 340);   
    cardDeck.cardBacks[2].setOrigin(xMid - 100, yMid + 340);   
    cardDeck.cardBacks[3].setOrigin(xMid + 200, yMid - 195);
    cardDeck.cardBacks[4].setOrigin(xMid +  50, yMid - 195);
    cardDeck.cardBacks[5].setOrigin(xMid - 100, yMid - 195);
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
        {100.f, 108.f},
        {250.f, 108.f},
        {400.f, 108.f},
        {100.f, 643.f},
        {250.f, 643.f},
        {400.f, 643.f}
    };

    for(short i = 0; i < numberOfPlayers; i++) {
        handSizeNumbers.push_back(headingText);
        handSizeNumbers[i].setFont(font); 
        handSizeNumbers[i].setCharacterSize(50); 
        handSizeNumbers[i].setFillColor(sf::Color(255, 255, 255));
        handSizeNumbers[i].setString(to_string(playerList[i]->numCardsInHand));

        // Makes the center of the text box the position
        sf::FloatRect textRect = handSizeNumbers[0].getLocalBounds();
        handSizeNumbers[i].setOrigin(textRect.left + textRect.width  / 2.0f, 
                                        textRect.top  + textRect.height / 2.0f);

        if(playerList[i]->hand.size() < 10) 
            textPositions[i].first += 1;

        handSizeNumbers[i].setPosition(sf::Vector2f(textPositions[i].first, textPositions[i].second));     
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::setAndPlaceVictoryText() {

    for(short i = 0; i < numberOfPlayers; i++) {
        roundResultPositions.push_back({-cardPositions[i].first + 50.f, -cardPositions[i].second + 72.f});
    }

    for(short i = 0; i < numberOfPlayers; i++) {
        winnerText.push_back(headingText);
        tieText.push_back(headingText);
        winnerText[i].setFont(font); 
        tieText[i].setFont(font);
        winnerText[i].setCharacterSize(70); 
        tieText[i].setCharacterSize(70);
        winnerText[i].setFillColor(sf::Color(60, 210, 60));
        tieText[i].setFillColor(sf::Color::Cyan);
        winnerText[i].setString("Winner");
        tieText[i].setString("Tie");

        // Makes the center of the text box the position
        sf::FloatRect textRect = winnerText[0].getLocalBounds();
        winnerText[i].setOrigin(textRect.left + textRect.width  / 2.0f, 
                                textRect.top  + textRect.height / 2.0f);

        tieText[i].setOrigin(textRect.left + textRect.width  / 2.0f, 
                                textRect.top  + textRect.height / 2.0f);                                        

        winnerText[i].setPosition(sf::Vector2f(roundResultPositions[i].first, roundResultPositions[i].second));  
        tieText[i].setPosition(sf::Vector2f(roundResultPositions[i].first + 46, roundResultPositions[i].second));   
    }
}

// -------------------------------------------------------------------------------------------------
// Looped
void GameTable::activateGameRound() {

    if(mayPlayNextRound) 
        revealFaceUpCardsWithDelayOf(150);

    // Determine if there is a winner or a tie
    if(faceupCards == numberOfPlayers && winnerNotYetDetermined) {
        getRoundWinner(playerList, 0); 
        winnerNotYetDetermined = false; // Makes sure to do this only once.
        clock.restart();
        if(winnerPool.size() == 1) 
            isAWinner = true;
        else
            isATie = true;
    }

    // Display winner/tie text
    if(isAWinner && clock.getElapsedTime().asMilliseconds() > 1000)
        declareWinner();

    if(isATie && clock.getElapsedTime().asMilliseconds() > 1000) {
        declareTie();
        clock.restart();
        isATie = false;
        mayBreakTie = true;
    }

    static short delay = 2500;
    if(mayBreakTie && clock.getElapsedTime().asMilliseconds() > delay) {
        static short winnerPoolIndex = 0;
        playTieBreakerCard(winnerPool[winnerPoolIndex]);
        delay = 250;
        clock.restart();
        winnerPoolIndex++;
        if(winnerPoolIndex >= winnerPool.size())
            mayBreakTie = false;
    }
}

// -------------------------------------------------------------------------------------------------
// Once
void GameTable::playTieBreakerCard(shared_ptr<Player> player) {
    playNextCardOnDeck(player);
    cout << "Checkpoint 1" << endl;
    // player.hand[0] = player.hand[1];
    // cout << player.name << "'s card = " << player.hand[0]->cardName << endl;
    cout << "Checkpoint 2"<< endl;
}

// -------------------------------------------------------------------------------------------------
// Once
void GameTable::declareWinner() {
    globalData->gameSound.playSoundEffect("winner2.ogg");
    mayDisplayWinnerText = true;
    isAWinner = false;
}

// -------------------------------------------------------------------------------------------------
// Once
void GameTable::declareTie() {
    globalData->gameSound.playSoundEffect("tie.ogg");
    mayDisplayTieText = true;
    isATie = false;

    clock.restart();
    if(elapsed.asMilliseconds() > 3000) {
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::getRoundWinner(vector<shared_ptr<Player>> competingPlayers, short ith_card) {

    short highestCard = getHighestCardValuePlayed(competingPlayers, ith_card);

    for(short i = 0; i < competingPlayers.size(); i++) {
        if(competingPlayers[i]->outOfGame)
            continue;

        short currentCard = competingPlayers[i]->hand[ith_card]->value;

        if(currentCard == highestCard) {
            shared_ptr<Player> playerPtr = competingPlayers[i];
            winnerPool.push_back(playerPtr);
        }
    }    

    if(winnerPool.size() > 1) 
        isATie = true;
}

// -------------------------------------------------------------------------------------------------

short GameTable::getHighestCardValuePlayed(vector<shared_ptr<Player>> competingPlayers, short ith_card) {
    short highestCard = 0;

    for(short i = 0; i < competingPlayers.size(); i++) {
        if(competingPlayers[i]->outOfGame)
            continue;

        short currentCard = competingPlayers[i]->hand[ith_card]->value;
        if(currentCard > highestCard)
            highestCard = currentCard;
    }

    return highestCard;
}

// -------------------------------------------------------------------------------------------------

void GameTable::drawCardsBacksAndNumbers() {
    for(short i = 0; i < numberOfPlayers; i++) {
        globalData->window.draw(greenRectangles[i]);
        globalData->window.draw(cardDeck.cardBacks[i]);
        globalData->window.draw(handSizeNumbers[i]);
        elapsed = clock.getElapsedTime();
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::revealFaceUpCardsWithDelayOf(short delay) {
    if(faceupCards >= playerList.size()) {
        winnerNotYetDetermined = true;
        mayPlayNextRound = false;
    }

    if(elapsed.asMilliseconds() > 150 && faceupCards < playerList.size()) {
        faceupCards++;
        clock.restart();
        playNextCardOnDeck(playerList[faceupCards - 1]);
    }
}


// -------------------------------------------------------------------------------------------------

void GameTable::playNextCardOnDeck(shared_ptr<Player> player) {
    player->numCardsInHand--;
    handSizeNumbers[player->number - 1].setString(to_string(player->numCardsInHand));
    playCardPlacementSound();
}

// -------------------------------------------------------------------------------------------------

void GameTable::drawPlayerFaceUpCards() {
    for(short i = 0; i < numberOfPlayers; i++) {
        if(i < faceupCards) 
            globalData->window.draw(playerList[i]->hand[0]->cardSprite);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::playCardPlacementSound() {
    short randNum = Miscellaneous::generateRandomNumber(2);
    globalData->gameSound.playSoundEffect("cardDeal" + to_string(randNum) + ".ogg");
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

void GameTable::dealCardsToPlayers() {
    vector<vector<shared_ptr<Card>>> hands;
    hands = cardDeck.divideDeck(numberOfPlayers);
    for(short i = 0; i < numberOfPlayers; i++) {
        playerList[i]->hand = hands[i];
        playerList[i]->numCardsInHand = hands[i].size();
    }

    if(forceTie) {
        for(auto i : cardDeck.deck) {
            if(i->value == 14 && playerList[0]->hand[0]->value < 14) 
                playerList[0]->hand[0] = i;
            playerList[1]->hand[0] = i;
        }
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

void GameTable::checkForMouseClickOnCard() {
    if(globalData->eventHandler.cardWasClicked) {
        mayPlayNextRound = true;
        globalData->eventHandler.cardWasClicked = false;
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::eventMonitor() {
    checkForMouseClickOnCard();
}

// -------------------------------------------------------------------------------------------------

void GameTable::printAllPlayerStats() {
    for(short i = 0; i < numberOfPlayers; i++) {
        cout << "\n==============================\n   Player " << i + 1 << " (Hand size = " 
                << playerList[i]->hand.size() << ")\n==============================\n";
        for(short j = 0; j < playerList[i]->hand.size(); j++) {
            cout << playerList[i]->hand[j]->cardName << "\t  {" << cardPositions[i].first << ", " 
                    << cardPositions[i].second << "}" << endl;
        }
    }
}