// Copyright Jonathan Carlson 2022

#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <iostream>
#include <vector>
#include "cardDeck.h"
#include "initializer.h"
#include "player.h"

using namespace std;

class GameTable {
public:

    GameTable() {}
    GameTable(Initializer & globalData);
    void runGameLoop();

private:

    short                      numberOfPlayers = 6;
    short                      faceupCards = 0;

    Initializer*               globalData;
    CardDeck                   cardDeck; 

    vector<Player>             playerList;
    vector<pair<float, float>> cardPositions;
    vector<pair<float, float>> roundResultPositions;
    vector<sf::Sprite>         greenRectangles;
    vector<Player>             winner;
    vector<sf::Text>           handSizeNumbers;
    vector<sf::Text>           winnerText;
    vector<sf::Text>           tieText;

    bool                       mayPlayNextRound       = false;
    bool                       winnerNotYetDetermined = false;
    bool                       mayDisplayTieText      = false;
    bool                       mayDisplayRoundResults = false;
    bool                       mayPlayVictorySound    = false;
    
    string                     fontFile = "Fonts/Robusta-Regular.ttf";
    float                      xMid, yMid;

	sf::Font                   font; 
    sf::Text                   headingText;
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
    void playCardPlacementSound();

    short getHighestCardValuePlayed(vector<Player> competingPlayers, short ith_card);
    void displayTieCondition();
    vector<Player> getRoundWinner  (vector<Player> competingPlayers, short ith_card);
    void declareWinner(Player winner);

    void printAllPlayerStats();
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
        for(short j = 0; j < playerList[i].hand.size(); j++) {
            playerList[i].hand[j].cardSprite.setOrigin(cardPositions[i].first, cardPositions[i].second);
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
        sf::Sprite sprite;
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
        handSizeNumbers[i].setString(to_string(playerList[i].numCardsInHand));

        // Makes the center of the text box the position
        sf::FloatRect textRect = handSizeNumbers[0].getLocalBounds();
        handSizeNumbers[i].setOrigin(textRect.left + textRect.width  / 2.0f, 
                                        textRect.top  + textRect.height / 2.0f);

        if(playerList[i].hand.size() < 10) 
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
        winnerText[i].setFont(font); 
        winnerText[i].setCharacterSize(70); 
        winnerText[i].setFillColor(sf::Color(60, 210, 60));
        winnerText[i].setString("Winner");

        // Makes the center of the text box the position
        sf::FloatRect textRect = winnerText[0].getLocalBounds();
        winnerText[i].setOrigin(textRect.left + textRect.width  / 2.0f, 
                                        textRect.top  + textRect.height / 2.0f);

        winnerText[i].setPosition(sf::Vector2f(roundResultPositions[i].first, roundResultPositions[i].second));     
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::activateGameRound() {

    if(mayPlayNextRound) 
        revealFaceUpCardsWithDelayOf(150);

    drawCardsBacksAndNumbers();

    if(faceupCards == numberOfPlayers && winnerNotYetDetermined) {
        winner = getRoundWinner(playerList, 0); 
        winnerNotYetDetermined = false; // Makes sure to do this only once.
        clock.restart();
        mayPlayVictorySound = true;
    }

    if(mayPlayVictorySound && clock.getElapsedTime().asMilliseconds() > 1000)
        declareWinner(winner[0]);

    if(mayDisplayRoundResults) 
        globalData->window.draw(winnerText[winner[0].number - 1]);

    if(mayDisplayTieText) {
        for(short i = 0; i < winner.size(); i++) {
            globalData->window.draw(tieText[winner[i].number - 1]);
        }
    }
    
}

// -------------------------------------------------------------------------------------------------

void GameTable::declareWinner(Player winner) {
    globalData->gameSound.playSoundEffect("winner2.ogg");
    mayDisplayRoundResults = true;
    mayPlayVictorySound = false;
}

// -------------------------------------------------------------------------------------------------

void GameTable::displayTieCondition() {

}

// -------------------------------------------------------------------------------------------------

vector<Player> GameTable::getRoundWinner(const vector<Player> competingPlayers, short ith_card) {

    vector<Player> winnerPool;
    short highestCard = getHighestCardValuePlayed(competingPlayers, ith_card);

    // Check for ties. Re-play if there is a tie.
    for(short i = 0; i < competingPlayers.size(); i++) {
        if(competingPlayers[i].outOfGame)
            continue;

        short currentCard = competingPlayers[i].hand[ith_card].value;

        if(currentCard == highestCard) 
            winnerPool.push_back(competingPlayers[i]);
    }    

    if(winnerPool.size() > 1) {
        mayDisplayTieText = true;
        displayTieCondition();
        winnerPool = getRoundWinner(winnerPool, ++ith_card);
    }

    return winnerPool;
}

// -------------------------------------------------------------------------------------------------

short GameTable::getHighestCardValuePlayed(vector<Player> competingPlayers, short ith_card) {
    short highestCard = 0;

    for(short i = 0; i < competingPlayers.size(); i++) {
        if(playerList[i].outOfGame)
            continue;

        short currentCard = competingPlayers[i].hand[ith_card].value;
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
        if(i < faceupCards) 
            globalData->window.draw(playerList[i].hand[0].cardSprite);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::revealFaceUpCardsWithDelayOf(short delay) {
    if(faceupCards >= numberOfPlayers) {
        winnerNotYetDetermined = true;
        mayPlayNextRound = false;
    }

    if(elapsed.asMilliseconds() > 150 && faceupCards < numberOfPlayers) {
        faceupCards++;
        clock.restart();
        playerList[faceupCards - 1].numCardsInHand--;
        handSizeNumbers[faceupCards - 1].setString(to_string(playerList[faceupCards - 1].numCardsInHand));
        playCardPlacementSound();
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
        playerList.push_back(Player());
        playerList[i].number = i + 1;
        playerList[i].name = "Player " + to_string(i + 1);
    }
}

// -------------------------------------------------------------------------------------------------

void GameTable::runGameLoop() {
    while(globalData->window.isOpen()) {
        globalData->eventHandler.listen();
        eventMonitor();
        globalData->window.clear(sf::Color(0, 90, 0));
        activateGameRound();
        globalData->window.display();
    } 
}   

// -------------------------------------------------------------------------------------------------

void GameTable::dealCardsToPlayers() {
    vector<vector<Card>> hands;
    hands = cardDeck.divideDeck(numberOfPlayers);
    for(short i = 0; i < numberOfPlayers; i++) {
        playerList[i].hand = hands[i];
        playerList[i].numCardsInHand = hands[i].size();
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
                << playerList[i].hand.size() << ")\n==============================\n";
        for(short j = 0; j < playerList[i].hand.size(); j++) {
            cout << playerList[i].hand[j].cardName << "\t  {" << cardPositions[i].first << ", " 
                    << cardPositions[i].second << "}" << endl;
        }
    }
}