// Copyright Jonathan Carlson 2022

#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "eventHandler.h"
#include "gameSound.h"
#include "textureManager.h"

class Initializer {
public:

	Initializer() : window(sf::VideoMode(screenWidth, screenHeight), "War Card Game By Jonathan Carlson"), 
					gameSound(), eventHandler(window, screenWidth, screenHeight, gameSound) {
						
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
	short frameRate           = 60;
	string defaultMusic       = "Sounds/Music/TownTheme.ogg";

	sf::RenderWindow window;
	// sf::Sprite background;

	GameSound      gameSound;
	EventHandler   eventHandler;
	TextureManager textures;	
};

#endif // INITIALIZER_H