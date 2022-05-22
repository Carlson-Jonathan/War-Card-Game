// Copyright Jonathan Carlson 2022

#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <iostream>
using namespace std;

#include "initializer.h"
#include "cardDeck.h"
#include "miscellaneous.h"

class TitleScreen {
public:

    TitleScreen() {}
    TitleScreen(Initializer & globalData);

    void titleScreenLoop();

private:

    Initializer* globalData;
    CardDeck cardDeck;
    vector<sf::Text> headingTextObjects;
    pair<float, float> firstCardPosition = {225.f, 350.f};
    bool buttonIsHeld = false;
    short numberOfPlayers = 6;

    vector<pair<float, float>> headingTextPositions = {
        {340,  50},
        {462, 110},
        { 25, 590},
        {144, 640}
    };

    pair<pair<float, float>, pair<float, float>> playerMinusClickArea = {{200, 230},{615, 645}};
    pair<pair<float, float>, pair<float, float>> playerPlusClickArea  = {{270, 300},{615, 645}};
    pair<pair<float, float>, pair<float, float>> startButtonClickArea = {{140, 300},{670, 720}};

    vector<string> headingText = {
        "War Card Game",
        "by Jonathan Carlson",
        "Players:\t-  " + to_string(numberOfPlayers) + "  +",
        "Start"
    };

    void set_CardPositions();
    void set_HeadingText();

    void draw_cardDisplay();
    void draw_AllText();

    void listen_ForMouseClicks();
    void listener_PlayerMinus(float x, float y);
    void listener_PlayerPlus (float x, float y);
    void listener_StartButton(float x, float y);

    bool leftClick();
    bool leftRelease();
    void verifyPlayersRange();

};


// =================================================================================================


TitleScreen::TitleScreen(Initializer & globalData) : cardDeck(globalData) {
    this->globalData = &globalData;
    set_CardPositions();
    set_HeadingText();
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::titleScreenLoop() {
    draw_cardDisplay();
    draw_AllText();
    listen_ForMouseClicks();
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::set_CardPositions() {
    for(short i = 0; i < cardDeck.deck.size(); i++) {
        cardDeck.deck[i]->cardSprite.setOrigin(sf::Vector2f(firstCardPosition.first, 
                                                            firstCardPosition.second));
        firstCardPosition.first  -= 20;
        firstCardPosition.second -= 30;
    }
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::draw_cardDisplay() {
    for(short i = 0; i < cardDeck.deck.size(); i++) {
        globalData->window.draw(cardDeck.deck[i]->cardSprite);
    }
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::set_HeadingText() {
    for(short i = 0; i < headingTextPositions.size(); i++) {    
        headingTextObjects.push_back(sf::Text());
        headingTextObjects[i].setFont(globalData->defaultFont); 
        headingTextObjects[i].setCharacterSize(50); 
        headingTextObjects[i].setFillColor(sf::Color(160, 240, 160));
        headingTextObjects[i].setString(headingText[i]);
        headingTextObjects[i].setPosition(sf::Vector2f(headingTextPositions[i].first, 
                                                       headingTextPositions[i].second));
    }

    Miscellaneous::centerTextAlignment(headingTextObjects[0]);
    Miscellaneous::centerTextAlignment(headingTextObjects[1]);

    headingTextObjects[1].setCharacterSize(25);
    headingTextObjects[2].setCharacterSize(50);
    headingTextObjects[3].setCharacterSize(75);

}

// -------------------------------------------------------------------------------------------------

void TitleScreen::draw_AllText() {
    for(short i = 0; i < headingText.size(); i++) {
        globalData->window.draw(headingTextObjects[i]);
    }
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::listen_ForMouseClicks() {

    float mouseX = sf::Mouse::getPosition(globalData->window).x;
    float mouseY = sf::Mouse::getPosition(globalData->window).y;

    if(leftClick()) {
        listener_PlayerMinus(mouseX, mouseY);
        listener_PlayerPlus (mouseX, mouseY);
        listener_StartButton(mouseX, mouseY);
    }

    if(leftRelease()) {
    }
}

// -------------------------------------------------------------------------------------------------


bool TitleScreen::leftClick() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonIsHeld) {
        buttonIsHeld = true;
        return true;
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

bool TitleScreen::leftRelease() {
    if(globalData->eventHandler.mouseRelease) {
        buttonIsHeld = false;
        globalData->eventHandler.mouseRelease = false;
        return true;
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::listener_PlayerMinus(float x, float y) {

    bool xBegin = x > playerMinusClickArea.first.first;
    bool xEnd   = x < playerMinusClickArea.first.second;
    bool yBegin = y > playerMinusClickArea.second.first;
    bool yEnd   = y < playerMinusClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        globalData->numberOfPlayers--;
        numberOfPlayers--;
        verifyPlayersRange();
        headingTextObjects[2].setString("Players:\t-  " + to_string(numberOfPlayers) + "  +");
        globalData->gameSound.playSoundEffect("tClick.ogg");
    }
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::listener_PlayerPlus(float x, float y) {

    bool xBegin = x > playerPlusClickArea.first.first;
    bool xEnd   = x < playerPlusClickArea.first.second;
    bool yBegin = y > playerPlusClickArea.second.first;
    bool yEnd   = y < playerPlusClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        globalData->numberOfPlayers++;
        numberOfPlayers++;
        verifyPlayersRange();
        headingTextObjects[2].setString("Players:\t-  " + to_string(numberOfPlayers) + "  +");
        globalData->gameSound.playSoundEffect("tClick.ogg");
    }
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::listener_StartButton(float x, float y) {
    bool xBegin = x > startButtonClickArea.first.first;
    bool xEnd   = x < startButtonClickArea.first.second;
    bool yBegin = y > startButtonClickArea.second.first;
    bool yEnd   = y < startButtonClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        globalData->mayInitializeGameTable = true;
        globalData->atTitleScreen = false;
        globalData->gameSound.playSoundEffect("tClick.ogg");
    }
}

// -------------------------------------------------------------------------------------------------

void TitleScreen::verifyPlayersRange() {
    if(globalData->numberOfPlayers > 6)
        globalData->numberOfPlayers = 2;
    if(globalData->numberOfPlayers < 2)
        globalData->numberOfPlayers = 6;
    if(numberOfPlayers > 6)
        numberOfPlayers = 2;
    if(numberOfPlayers < 2)
        numberOfPlayers = 6;        
}

// -------------------------------------------------------------------------------------------------

#endif // TITLESCREEN_H