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
    vector<pair<float, float>> menuItemPositions = {
        {250.f, 65.f},
        {30.f, 100.f},
        {30.f, 150.f},
        {30.f, 200.f},
        {30.f, 250.f},
        {30.f, 300.f},
        {30.f, 350.f},
        {30.f, 400.f}                        
    };

    //  {{x.begin, x.end}, {y.begin, y.end}}
    vector<pair<float, float>> xClickZonesMenuSpeed = {
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

    vector<float> gameSpeedXCoordinates = {
        176, 201, 229, 257, 286, 314, 342, 371, 400, 433
    };

    vector<string> menuItemStrings = {
        "Settings",
        "Number of Players:\t6",
        "Game Speed:\t1  2  3  4  5  6  7  8  9  10",
        "Auto Click:\tDisabled",
        "Card Style:\tClassic\tElla Style",
        "Music Volume:\t100%",
        "Sound Volume:\t100%",
        "Credits"
    };    

    void setMenuItemPositions();
    bool leftClicked();
    void drawSelectBox(short xPos);
    void test();
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
    test();
    drawSelectBox(gameSpeedSelection);
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
        cout << "Click!" << endl;
        return true;
    }

    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonIsHeld) 
        buttonIsHeld = false;


    return false;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::drawSelectBox(short xPos) { 
    sf::RectangleShape rectangle(sf::Vector2f(24.f, 50.f));
    rectangle.setFillColor(sf::Color(0.f, 50.f, 0.f));
    rectangle.setOrigin(-xClickZonesMenuSpeed[xPos].first, -menuItemPositions[2].second);
    globalData->window.draw(rectangle);
}

// -------------------------------------------------------------------------------------------------

void GameMenu::test() {

    float mouseX = sf::Mouse::getPosition(globalData->window).x;
    float mouseY = sf::Mouse::getPosition(globalData->window).y;

    bool xBegin, xEnd, yBegin, yEnd = false;

    if(leftClicked()) {

        cout << "\n{" << mouseX << ", " << mouseY << "}\n" << endl;

        for(short i = 0; i < 10; i++) {

            xBegin = mouseX > xClickZonesMenuSpeed[i].first;
            xEnd   = mouseX < xClickZonesMenuSpeed[i].second;
            yBegin = mouseY > menuItemPositions[2].second;
            yEnd   = mouseY < menuItemPositions[2].second + 50;

            cout << "================= " << i << " =================" << endl;
            cout << "xBegin: " << xBegin << "  xEnd: " << xEnd << "\t{" << xClickZonesMenuSpeed[i].first << ", " << xClickZonesMenuSpeed[i].second << "}" << endl;
            cout << "yBegin: " << yBegin << "  yEnd: " << yEnd << "\t{" << menuItemPositions[2].second << ", " << menuItemPositions[2].second + 50 << "}" << endl;

            if(xBegin && xEnd && yBegin && yEnd) {
                gameSpeedSelection = i;
                globalData->gameSpeed = i;
                globalData->gameSound.playSoundEffect("tClick.ogg");
            }
        }
    }
}