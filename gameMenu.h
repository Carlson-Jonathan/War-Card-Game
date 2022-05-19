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

    sf::RectangleShape musicVolumeBarOutline;
    sf::RectangleShape musicVolumeBarFiller;
    sf::RectangleShape soundVolumeBarOutline;
    sf::RectangleShape soundVolumeBarFiller;    

    bool buttonIsHeld = false;
    short gameSpeedSelection = 5;

    vector<sf::Text> menuItemTexts;
    vector<string> const menuItemStrings = {
        "Settings",
        "Number of Players:\t6",
        "Game Speed:\t1  2  3  4  5  6  7  8  9  10",
        "Auto Play:\tDisabled",
        "Card Style:\tRed\t\tBlue\t\tGreen",
        "Music Volume:\t\t\t\t30%",
        "Sound Volume:\t\t\t\t100%",
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
    pair<pair<float, float>, pair<float, float>> autoClickArea   {{180, 300}, {210, 250}};     
    pair<pair<float, float>, pair<float, float>> musicVolumeArea {{200, 465}, {308, 347}};

    void setMenuItemPositions();
    void setAndDrawGameSpeedSelectionBox(short xPos);
    void setMusicVolumeBar();
    void drawVolumeBars();

    void listenForMouseClicks();
    bool leftClicked();

    void setGameSpeed(float x, float y);
    void toggleAutoClick(float x, float y);
    void adjustMusicVolume(float x, float y);

    float calculateAreaPercentage(short x);

};

#endif // GAMEMENU_H


// =================================================================================================


GameMenu::GameMenu(Initializer & globalData) {
    this->globalData = &globalData;
    setMenuItemPositions();
    setMusicVolumeBar();
}

// -------------------------------------------------------------------------------------------------

void GameMenu::gameMenuLoop() {
    listenForMouseClicks();
    setAndDrawGameSpeedSelectionBox(gameSpeedSelection);
    drawVolumeBars();


    // This stays last
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
        adjustMusicVolume(mouseX, mouseY);
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

// -------------------------------------------------------------------------------------------------

void GameMenu::setMusicVolumeBar() {
    musicVolumeBarOutline.setSize(sf::Vector2f(260.f, 32.f));
    musicVolumeBarOutline.setOrigin(-xClickZonesMenuSpeed[1].first, -menuItemPositions[5].second - 9);
    musicVolumeBarOutline.setFillColor(sf::Color(0.f, 90.f, 0.f));
    musicVolumeBarOutline.setOutlineThickness(4.f);
    musicVolumeBarOutline.setOutlineColor(sf::Color(0, 50, 0));    

    musicVolumeBarFiller.setSize(sf::Vector2f(81.f, 36.f));
    musicVolumeBarFiller.setOrigin (-xClickZonesMenuSpeed[1].first, -menuItemPositions[5].second - 5);
    musicVolumeBarFiller.setFillColor(sf::Color(0.f, 50.f, 0.f));

    soundVolumeBarOutline.setSize(sf::Vector2f(260.f, 32.f));
    soundVolumeBarOutline.setOrigin(-xClickZonesMenuSpeed[1].first, -menuItemPositions[6].second - 9);
    soundVolumeBarOutline.setFillColor(sf::Color(0.f, 90.f, 0.f));
    soundVolumeBarOutline.setOutlineThickness(4.f);
    soundVolumeBarOutline.setOutlineColor(sf::Color(0, 50, 0));     

    soundVolumeBarFiller.setSize(sf::Vector2f(80.f, 36.f));
    soundVolumeBarFiller.setOrigin (-xClickZonesMenuSpeed[1].first, -menuItemPositions[6].second - 5);
    soundVolumeBarFiller.setFillColor(sf::Color(0.f, 50.f, 0.f));    
}

// -------------------------------------------------------------------------------------------------

void GameMenu::drawVolumeBars() {
    // x - 200-465
    // y - 308-347
    globalData->window.draw(musicVolumeBarOutline);
    globalData->window.draw(musicVolumeBarFiller);
    globalData->window.draw(soundVolumeBarOutline);
    globalData->window.draw(soundVolumeBarFiller);    
}

// -------------------------------------------------------------------------------------------------

void GameMenu::adjustMusicVolume(float x, float y) {
   
    bool xBegin, xEnd, yBegin, yEnd = false;
    float newPercent = 0;
    float newWidth = 0.0;
    string newString = "";

    xBegin = x > musicVolumeArea.first.first;
    xEnd   = x < musicVolumeArea.first.second;
    yBegin = y > musicVolumeArea.second.first;
    yEnd   = y < musicVolumeArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        newWidth = x - xClickZonesMenuSpeed[1].first;
        musicVolumeBarFiller.setSize(sf::Vector2f(newWidth, 36.f));
        newPercent = (x - musicVolumeArea.first.first) / (musicVolumeArea.first.second - 200) * 100;
        newString = "Music Volume:\t\t\t\t" + to_string(short(newPercent)) + "%";
        menuItemTexts[5].setString(newString);
        buttonIsHeld = false;
        globalData->gameSound.music.setVolume(newPercent);
    }    
}

// -------------------------------------------------------------------------------------------------
