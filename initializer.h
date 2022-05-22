// Copyright Jonathan Carlson 2022

#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "gameSound.h"
#include "eventHandler.h"
#include "textureManager.h"

using namespace std;

class Initializer {
public:

	Initializer() : window(sf::VideoMode(screenWidth, screenHeight), "War Card Game By Jonathan Carlson"), 
					eventHandler(window, screenWidth, screenHeight) {		
						
		window.setFramerateLimit(frameRate);

		// Set background texture
		// background.setTexture(textures.textures["sample2"]);  
		// background.setOrigin(0, 0);	

		// Play song
		gameSound.loadAndPlayMusic(defaultMusic);
		setFontFamily();
	}

    //----------------------------------------------------------------------------------------------

	unsigned int screenWidth  = 500;
	unsigned int screenHeight = 750;
	
	short frameRate = 20;
	short gameSpeed	= 4;
	short numberOfPlayers = 6;

	string defaultMusic      = "Sounds/Music/TownTheme.ogg";
	string defaultFontFamily = "Fonts/Robusta-Regular.ttf";
	string cardBack          = "redCardBack";

	bool gameMenuIsOpen = false;
	bool atTitleScreen  = true;
	bool autoClick      = false;	
	bool mayInitializeGameTable = false;

	sf::RenderWindow window;
	sf::Font defaultFont;
	// sf::Sprite background;

	GameSound      gameSound;
	EventHandler   eventHandler;
	TextureManager textures;

private:

	void setFontFamily() {
		if (!this->defaultFont.loadFromFile(this->defaultFontFamily)) {
			cout << "ERROR: GameTable::setText(): Font " << defaultFontFamily << " not found." << endl; 
			exit(139);
		}   
	}
};


// =================================================================================================


#endif // INITIALIZER_H