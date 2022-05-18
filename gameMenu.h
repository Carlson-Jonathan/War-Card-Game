// Copyright Jonathan Carlson 2022

#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <iostream>
using namespace std;

#include "initializer.h"
#include "miscellaneous.h"

class GameMenu {
public: 

    GameMenu() {}
    GameMenu(Initializer & globalData);

    void gameMenuLoop();

private:

    Initializer* globalData;

    bool buttonIsHeld = false;
    short gameSpeedSelection = 5;

    vector<sf::Text> menuItemTexts;
    vector<string> const menuItemStrings = {
        "Settings",
        "Number of Players:\t6",
        "Game Speed:\t1  2  3  4  5  6  7  8  9  10",
        "Auto Play:\tDisabled",
        "Card Style:\tRed\tBlue\tGreen",
        "Music Volume:\t100%",
        "Sound Volume:\t100%",
        "Credits"
    }; 

    vector<pair<float, float>> const menuItemPositions = {
        {250.f, 65.f},
        {30.f, 100.f},
        {30.f, 150.f},
        {30.f, 200.f},
        {30.f, 250.f},
        {30.f, 300.f},
        {30.f, 350.f},
        {30.f, 400.f}                        
    };

    //  {{x.begin, x.end}
    vector<pair<float, float>> const xClickZonesMenuSpeed = {
        {176, 200},
        {201, 225},
        {229, 253},
        {257, 281},
        {286, 310},
        {314, 338},
        {342, 366},
        {371, 395},
        {400, 424},
        {433, 457}
    };

    // {x.begin, x.end}, {y.begin, y.end}
    pair<pair<float, float>, pair<float, float>> autoClickArea {
        {180, 300}, {210, 250}
    };

    void listenForMouseClicks();
    void setMenuItemPositions();
    bool leftClicked();
    void setAndDrawGameSpeedSelectionBox(short xPos);
    void setGameSpeed(float x, float y);
    void toggleAutoClick(float x, float y);
};

#endif // GAMEMENU_H


// =================================================================================================


GameMenu::GameMenu(Initializer & globalData) {
    this->globalData = &globalData;
    setMenuItemPositions();
    cout << menuItemPositions.size() << endl;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::gameMenuLoop() {
    listenForMouseClicks();
    setAndDrawGameSpeedSelectionBox(gameSpeedSelection);
    for(short i = 0; i < menuItemTexts.size(); i++) {
        globalData->window.draw(menuItemTexts[i]);
    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::setMenuItemPositions() {
    for(short i = 0; i < menuItemPositions.size(); i++) {
        menuItemTexts.push_back(sf::Text());
        menuItemTexts[i].setFont(globalData->defaultFont); 
        menuItemTexts[i].setCharacterSize(35); 
        menuItemTexts[i].setFillColor(sf::Color(160, 240, 160));
        menuItemTexts[i].setString(menuItemStrings[i]);
        menuItemTexts[i].setPosition(sf::Vector2f(menuItemPositions[i].first, 
                                                  menuItemPositions[i].second));
    }

    menuItemTexts[0].setCharacterSize(55); 
    Miscellaneous::centerTextAlignment(menuItemTexts[0]);
}

// -------------------------------------------------------------------------------------------------

bool GameMenu::leftClicked() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonIsHeld) {
        buttonIsHeld = true;
        return true;
    }

    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonIsHeld) 
        buttonIsHeld = false;


    return false;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::setAndDrawGameSpeedSelectionBox(short xPos) { 
    sf::RectangleShape rectangle(sf::Vector2f(24.f, 50.f));
    rectangle.setFillColor(sf::Color(0.f, 50.f, 0.f));
    rectangle.setOrigin(-xClickZonesMenuSpeed[xPos].first, -menuItemPositions[2].second);
    globalData->window.draw(rectangle);
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listenForMouseClicks() {

    float mouseX = sf::Mouse::getPosition(globalData->window).x;
    float mouseY = sf::Mouse::getPosition(globalData->window).y;

    if(leftClicked()) {
        setGameSpeed   (mouseX, mouseY);
        toggleAutoClick(mouseX, mouseY);

    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::setGameSpeed(float x, float y) {

    bool xBegin, xEnd, yBegin, yEnd = false;

    for(short i = 0; i < 10; i++) {

        xBegin = x > xClickZonesMenuSpeed[i].first;
        xEnd   = x < xClickZonesMenuSpeed[i].second;
        yBegin = y > menuItemPositions[2].second;
        yEnd   = y < menuItemPositions[2].second + 50;

        // cout << "================= " << i << " =================" << endl;
        // cout << "xBegin: " << xBegin << "  xEnd: " << xEnd << "\t{" << xClickZonesMenuSpeed[i].first << ", " << xClickZonesMenuSpeed[i].second << "}" << endl;
        // cout << "yBegin: " << yBegin << "  yEnd: " << yEnd << "\t{" << menuItemPositions[2].second << ", " << menuItemPositions[2].second + 50 << "}" << endl;

        if(xBegin && xEnd && yBegin && yEnd) {
            gameSpeedSelection = i;
            globalData->gameSpeed = i;
            globalData->gameSound.playSoundEffect("tClick.ogg");
        }
    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::toggleAutoClick(float x, float y) {
   
    bool xBegin, xEnd, yBegin, yEnd = false;
    static short state = 0;
    string autoClick = "";

    cout << "{" << x << ", " << y << "}" << endl;

    xBegin = x > autoClickArea.first.first;
    xEnd   = x < autoClickArea.first.second;
    yBegin = y > autoClickArea.second.first;
    yEnd   = y < autoClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {

        if(state++ % 2) {
            autoClick = "Auto Play:\tDisabled";
            globalData->autoClick = false;
        }
        else {
            autoClick = "Auto Play:\tEnabled";
            globalData->autoClick = true;
        }

        menuItemTexts[3].setString(autoClick);
        globalData->gameSound.playSoundEffect("tClick.ogg");
    }
}
