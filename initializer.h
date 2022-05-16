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
					eventHandler(window, screenWidth, screenHeight, gameSound) {
						
		window.setFramerateLimit(frameRate);

		// Set background texture
		// background.setTexture(textures.textures["sample2"]);  
		// background.setOrigin(0, 0);	

		// Play song
		gameSound.loadAndPlayMusic(defaultMusic);
	}

    //----------------------------------------------------------------------------------------------

	unsigned int screenWidth  = 500;
	unsigned int screenHeight = 750;
	short frameRate           = 20;
	string defaultMusic       = "Sounds/Music/TownTheme.ogg";
	bool gameMenuIsOpen       = false;

	sf::RenderWindow window;
	// sf::Sprite background;

	GameSound      gameSound;
	EventHandler   eventHandler;
	TextureManager textures;
};

#endif // INITIALIZER_H