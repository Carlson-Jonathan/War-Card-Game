// Copyright Jonathan Carlson 2022

#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <iostream>
using namespace std;

#include "initializer.h"

class GameMenu {
public: 

    GameMenu() {}
    GameMenu(Initializer & globalData);

    void gameMenuLoop();
    void updateNumberOfPlayersText(short number);

private:

    Initializer* globalData;

    bool soundVolumeWasAdjusted = false;
    bool buttonIsHeld = true;

    short gameSpeedSelection = 4;

    sf::RectangleShape musicVolumeBarOutline;
    sf::RectangleShape musicVolumeBarFiller;
    sf::RectangleShape soundVolumeBarOutline;
    sf::RectangleShape soundVolumeBarFiller;  
    sf::RectangleShape gameSpeedSelector;  

    sf::Sprite gearMenuIcon;

    vector<sf::Text> menuItemTexts;
    vector<string> menuItemStrings = {
        "Settings",
        "Number of Players:\t6",
        "Game Speed:\t1  2  3  4  5  6  7  8  9  10",
        "Auto Play:\tDisabled",
        "Card Style:",
        "Music Volume:\t\t\t\t30%",
        "Sound Volume:\t\t\t\t100%"
    };

    vector<sf::Text> creditItemTexts;
    vector<string> creditsItemStrings = {
        "\t\t\t\t\t\t\t\t\t< Credits >",
        "Programmer:\t\tJonathan Carlson",
        "Music:\t\t\t\t\tcynicmusic.com,\tpixelsphere.org",
        "Sound Effects:\t Kenney Vleugels,\tDavid McKee",
        "Images:\t\t\t\t  Byron Knoll",
        "\t\t\tSpecial Thanks to OpenGameArt.org!"
    }; 

    pair<pair<float, float>, pair<float, float>> gearIconClickArea = {
        {10, 40}, {10, 40}
    };

    vector<pair<float, float>> const menuTextPositions = {
        {250.f, 65.f},
        {30.f, 100.f},
        {30.f, 150.f},
        {30.f, 200.f},
        {30.f, 250.f},
        {30.f, 300.f},
        {30.f, 350.f}
    };

    vector<pair<float, float>> const creditTextPositions = {
        {30.f, 440.f},   
        {30.f, 480.f}, 
        {30.f, 520.f}, 
        {30.f, 560.f}, 
        {30.f, 600.f},
        {30.f, 660.f}                      
    };

    // Represents left and right most clickable edges of each number of game speed.
    vector<pair<float, float>> const gameSpeedClickArea_x = {
        {174, 200},
        {199, 225},
        {227, 255},
        {257, 284},
        {284, 312},
        {312, 340},
        {340, 369},
        {369, 398},
        {398, 429},
        {431, 457}
    };

    vector<sf::RectangleShape> cardStyleColorBoxes = {};
    vector<pair<float, float>> const xClickZonesCardStyle = {
        {197, 237},
        {254, 294},
        {311, 351},
        {368, 408},
        {425, 465}
    };

    vector<string> cardStyles = {
        "redCardBack",
        "blueCardBack",
        "blackCardBack",
        "orangeCardBack",
        "purpleCardBack"
    };

    vector<vector<float>> colors = {
        {255,   0,   0}, // Red
        {  0,   0, 255}, // Blue
        {  0,   0,   0}, // Black
        {224,  82,  15}, // Orange
        {186,  33, 189}  // Purple
    };

    // {x.begin, x.end}, {y.begin, y.end}
    pair<pair<float, float>, pair<float, float>> autoClickArea   {{180, 300}, {210, 250}};     
    pair<pair<float, float>, pair<float, float>> musicVolumeArea {{200, 465}, {308, 347}};
    pair<pair<float, float>, pair<float, float>> soundVolumeArea {{200, 465}, {360, 397}};

// -------------------------------------------------------------------------------------------------

    void set_GearMenuIcon();
    void set_AllMenuItemPositions();
    void set_GameSpeedSelector();
    void set_VolumeBars();
    void set_CardStyleColorBoxes();

    void draw_MenuItemTexts();
    void draw_GameSpeedSelector(short xPos);
    void draw_VolumeBars();
    void draw_CardStyleColorBoxes();

    void listen_ForMouseClicks();
    void listener_GearMenuIconClick(float x, float y);
    void listener_GameSpeedGUI(float x, float y);
    void listener_AutoPlayToggler(float x, float y);
    void listener_CardStyleGUI(float x, float y);
    void listener_MusicVolumeAdjuster(float x, float y);
    void listener_SoundVolumeAdjuster(float x, float y);    

    bool leftClick();
    bool leftRelease();
};


// =================================================================================================


GameMenu::GameMenu(Initializer & globalData) {
    this->globalData = &globalData;
    set_GearMenuIcon();
    set_AllMenuItemPositions();
    set_GameSpeedSelector();
    set_VolumeBars();
    set_CardStyleColorBoxes();
}

// -------------------------------------------------------------------------------------------------

void GameMenu::gameMenuLoop() {
    listen_ForMouseClicks();
    globalData->window.draw(gearMenuIcon);
    draw_GameSpeedSelector(gameSpeedSelection);
    draw_VolumeBars();
    draw_CardStyleColorBoxes();
    draw_MenuItemTexts();
}

// -------------------------------------------------------------------------------------------------

void GameMenu::set_GearMenuIcon() {
	gearMenuIcon.setTextureRect(sf::IntRect(0, 0, 30, 30));
    gearMenuIcon.setTexture(globalData->textures.textures["gearMenuIcon"]);
    gearMenuIcon.setOrigin(-10, -10);    
}

// -------------------------------------------------------------------------------------------------

void GameMenu::set_AllMenuItemPositions() {
    for(short i = 0; i < menuTextPositions.size(); i++) {
        menuItemTexts.push_back(sf::Text());
        menuItemTexts[i].setFont(globalData->defaultFont); 
        menuItemTexts[i].setCharacterSize(35); 
        menuItemTexts[i].setFillColor(sf::Color(160, 240, 160));
        menuItemTexts[i].setString(menuItemStrings[i]);
        menuItemTexts[i].setPosition(sf::Vector2f(menuTextPositions[i].first, 
                                                  menuTextPositions[i].second));
    }

    for(short i = 0; i < creditTextPositions.size(); i++) {
        creditItemTexts.push_back(sf::Text());
        creditItemTexts[i].setFont(globalData->defaultFont); 
        creditItemTexts[i].setCharacterSize(25); 
        creditItemTexts[i].setFillColor(sf::Color(160, 240, 160));
        creditItemTexts[i].setString(creditsItemStrings[i]);
        creditItemTexts[i].setPosition(sf::Vector2f(creditTextPositions[i].first, 
                                                    creditTextPositions[i].second));
    }    

    menuItemTexts[0].setCharacterSize(55); 
    Miscellaneous::centerTextAlignment(menuItemTexts[0]);
}

// -------------------------------------------------------------------------------------------------

void GameMenu::set_GameSpeedSelector() {
    gameSpeedSelector.setSize(sf::Vector2f(27.f, 39.f));
    gameSpeedSelector.setFillColor(sf::Color(0.f, 50.f, 0.f));
}

// -------------------------------------------------------------------------------------------------

void GameMenu::set_CardStyleColorBoxes() {

    for(short i = 0; i < xClickZonesCardStyle.size(); i++) {
        sf::RectangleShape rectangle(sf::Vector2f(40.f, 30.f));
        rectangle.setFillColor(sf::Color(0.f, 50.f, 0.f));
        rectangle.setOrigin(-xClickZonesCardStyle[i].first, -menuTextPositions[4].second - 10);
        rectangle.setFillColor(sf::Color(colors[i][0], colors[i][1], colors[i][2]));
        rectangle.setOutlineColor(sf::Color(160.f, 240.f, 160.f));
        cardStyleColorBoxes.push_back(rectangle);
    }  
        
    cardStyleColorBoxes[0].setOutlineThickness(4.f); // default (red)
}

// -------------------------------------------------------------------------------------------------

void GameMenu::set_VolumeBars() {
    musicVolumeBarOutline.setSize(sf::Vector2f(260.f, 32.f));
    musicVolumeBarOutline.setOrigin(-gameSpeedClickArea_x[1].first, -menuTextPositions[5].second - 9);
    musicVolumeBarOutline.setFillColor(sf::Color(0.f, 90.f, 0.f));
    musicVolumeBarOutline.setOutlineThickness(4.f);
    musicVolumeBarOutline.setOutlineColor(sf::Color(0, 50, 0));    

    musicVolumeBarFiller.setSize(sf::Vector2f(81.f, 36.f));
    musicVolumeBarFiller.setOrigin (-gameSpeedClickArea_x[1].first, -menuTextPositions[5].second - 5);
    musicVolumeBarFiller.setFillColor(sf::Color(0.f, 50.f, 0.f));

    soundVolumeBarOutline.setSize(sf::Vector2f(260.f, 32.f));
    soundVolumeBarOutline.setOrigin(-gameSpeedClickArea_x[1].first, -menuTextPositions[6].second - 9);
    soundVolumeBarOutline.setFillColor(sf::Color(0.f, 90.f, 0.f));
    soundVolumeBarOutline.setOutlineThickness(4.f);
    soundVolumeBarOutline.setOutlineColor(sf::Color(0, 50, 0));     

    soundVolumeBarFiller.setSize(sf::Vector2f(260.f, 36.f));
    soundVolumeBarFiller.setOrigin (-gameSpeedClickArea_x[1].first, -menuTextPositions[6].second - 5);
    soundVolumeBarFiller.setFillColor(sf::Color(0.f, 50.f, 0.f));    
}

// -------------------------------------------------------------------------------------------------

void GameMenu::draw_CardStyleColorBoxes() {
    for(short i = 0; i < cardStyleColorBoxes.size(); i++) {
        globalData->window.draw(cardStyleColorBoxes[i]);
    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::draw_MenuItemTexts() {
    for(short i = 0; i < menuItemTexts.size(); i++) {
        globalData->window.draw(menuItemTexts[i]);
    } 

    for(short i = 0; i < creditItemTexts.size(); i++) {
        globalData->window.draw(creditItemTexts[i]);
    } 
}

// -------------------------------------------------------------------------------------------------

bool GameMenu::leftClick() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonIsHeld) {
        buttonIsHeld = true;
        return true;
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

bool GameMenu::leftRelease() {
    if(globalData->eventHandler.mouseRelease) {
        buttonIsHeld = false;
        globalData->eventHandler.mouseRelease = false;
        return true;
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

void GameMenu::draw_GameSpeedSelector(short xPos) { 
    gameSpeedSelector.setOrigin(-gameSpeedClickArea_x[xPos].first, -menuTextPositions[2].second - 5);
    globalData->window.draw(gameSpeedSelector);
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listen_ForMouseClicks() {

    float mouseX = sf::Mouse::getPosition(globalData->window).x;
    float mouseY = sf::Mouse::getPosition(globalData->window).y;

    if(leftClick()) {
        listener_GearMenuIconClick   (mouseX, mouseY);
        listener_GameSpeedGUI        (mouseX, mouseY);
        listener_AutoPlayToggler     (mouseX, mouseY);
        listener_MusicVolumeAdjuster (mouseX, mouseY);
        listener_SoundVolumeAdjuster (mouseX, mouseY);
        listener_CardStyleGUI        (mouseX, mouseY);
    }

    if(leftRelease()) {
        if(soundVolumeWasAdjusted) {
            globalData->gameSound.playSoundEffect("winner.ogg");
            soundVolumeWasAdjusted = false;
        } 
    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listener_GameSpeedGUI(float x, float y) {

    for(short i = 0; i < 10; i++) {

        bool xBegin = x > gameSpeedClickArea_x[i].first;
        bool xEnd   = x < gameSpeedClickArea_x[i].second;
        bool yBegin = y > menuTextPositions[2].second;
        bool yEnd   = y < menuTextPositions[2].second + 50;

        if(xBegin && xEnd && yBegin && yEnd) {
            gameSpeedSelection = i;
            globalData->gameSpeed = i;
            globalData->gameSound.playSoundEffect("tClick.ogg");
        }
    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listener_AutoPlayToggler(float x, float y) {
   
    static short state = 0;
    string autoClick = "";

    bool xBegin = x > autoClickArea.first.first;
    bool xEnd   = x < autoClickArea.first.second;
    bool yBegin = y > autoClickArea.second.first;
    bool yEnd   = y < autoClickArea.second.second;

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

void GameMenu::listener_CardStyleGUI(float x, float y) {

    for(short i = 0; i < 5; i++) {

        bool xBegin = x > xClickZonesCardStyle[i].first;
        bool xEnd   = x < xClickZonesCardStyle[i].second;
        bool yBegin = y > menuTextPositions[4].second + 10;
        bool yEnd   = y < menuTextPositions[4].second + 40;

        if(xBegin && xEnd && yBegin && yEnd) {
            globalData->gameSound.playSoundEffect("tClick.ogg");
            globalData->cardBack = cardStyles[i];

            for(short i = 0; i < cardStyleColorBoxes.size(); i++) {
                cardStyleColorBoxes[i].setOutlineThickness(0.f); 
            }
            cardStyleColorBoxes[i].setOutlineThickness(4.f); 
        }
    } 
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listener_MusicVolumeAdjuster(float x, float y) {
   
    float newPercent = 0;
    float newWidth = 0.0;
    string newString = "";

    bool xBegin = x > musicVolumeArea.first.first;
    bool xEnd   = x < musicVolumeArea.first.second;
    bool yBegin = y > musicVolumeArea.second.first;
    bool yEnd   = y < musicVolumeArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        newWidth = x - gameSpeedClickArea_x[1].first;
        musicVolumeBarFiller.setSize(sf::Vector2f(newWidth, 36.f));
        newPercent = (x - musicVolumeArea.first.first) / (musicVolumeArea.first.second - 200) * 100;
        if(newPercent <= 1) newPercent = 0;
        if(newPercent >= 99) newPercent = 100;
        newString = "Music Volume:\t\t\t\t" + to_string(short(newPercent)) + "%";
        menuItemTexts[5].setString(newString);
        buttonIsHeld = false;
        globalData->gameSound.music.setVolume(newPercent);
    }    
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listener_SoundVolumeAdjuster(float x, float y) {
   
    float newPercent = 0;
    float newWidth = 0.0;
    string newString = "";

    bool xBegin = x > soundVolumeArea.first.first;
    bool xEnd   = x < soundVolumeArea.first.second;
    bool yBegin = y > soundVolumeArea.second.first;
    bool yEnd   = y < soundVolumeArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        newWidth = x - gameSpeedClickArea_x[1].first;
        soundVolumeBarFiller.setSize(sf::Vector2f(newWidth, 36.f));
        newPercent = (x - soundVolumeArea.first.first) / (soundVolumeArea.first.second - 200) * 100;
        if(newPercent <= 1) newPercent = 0;
        if(newPercent >= 99) newPercent = 100;
        newString = "Sound Volume:\t\t\t\t" + to_string(short(newPercent)) + "%";
        menuItemTexts[6].setString(newString);
        buttonIsHeld = false;
        globalData->gameSound.globalSoundEffectVolume = newPercent;
        soundVolumeWasAdjusted = true;

        // Prevents sound spam when dragging the volume control.
        if(globalData->eventHandler.mouseRelease) {
            globalData->eventHandler.mouseRelease = false;
        }
    }    
}

// -------------------------------------------------------------------------------------------------

void GameMenu::listener_GearMenuIconClick(float x, float y) {

    bool xBegin = x > gearIconClickArea.first.first;
    bool xEnd   = x < gearIconClickArea.first.second;
    bool yBegin = y > gearIconClickArea.second.first;
    bool yEnd   = y < gearIconClickArea.second.second;

    if(xBegin && xEnd && yBegin && yEnd) {
        globalData->gameSound.playSoundEffect("tClick.ogg");
        globalData->gameMenuIsOpen = false;
    }
}

// -------------------------------------------------------------------------------------------------

void GameMenu::updateNumberOfPlayersText(short number) {
    menuItemTexts[1].setString("Number of Players:\t" + to_string(number));
}

// -------------------------------------------------------------------------------------------------

void GameMenu::draw_VolumeBars() {
    globalData->window.draw(musicVolumeBarOutline);
    globalData->window.draw(musicVolumeBarFiller);
    globalData->window.draw(soundVolumeBarOutline);
    globalData->window.draw(soundVolumeBarFiller);    
}

#endif // GAMEMENU_H