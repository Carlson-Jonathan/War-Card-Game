// Copyright Jonathan Carlson 2022

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "gameSound.h"
#include "joystick.h"

using namespace std;

class EventHandler {
public:

	EventHandler() {}
	EventHandler(sf::RenderWindow & window, unsigned int & screenWidth, 
				 unsigned int & screenHeight, GameSound & gameSound);

	void listen();

private:

	sf::RenderWindow* window;
	unsigned int*  	  screenWidth;
	unsigned int*     screenHeight;
	GameSound* 		  gameSound;
	sf::Event 	 	  event;
	Joystick 		  joystick;

	void mouseButton();
	void closeWindow();
	void resizeWindow();
	void makeNoise();
};

#endif // EVENTHANDLER_H


// =================================================================================================


EventHandler::EventHandler(sf::RenderWindow & window, unsigned int & screenWidth, 
						   unsigned int & screenHeight, GameSound & gameSound) {
	this->window = &window;
	this->screenWidth = &screenWidth;
	this->screenHeight = &screenHeight;
	this->gameSound = &gameSound;
}

// -------------------------------------------------------------------------------------------------

void EventHandler::listen() {
	while (window->pollEvent(event)) {

		switch(event.type) {
			case sf::Event::Closed:
				closeWindow();
				break;
			case sf::Event::Resized:
				resizeWindow();
				break;
			case sf::Event::MouseButtonPressed:
				mouseButton();
				break;
			case sf::Event::MouseButtonReleased:
				cout << "Released!" << endl;
				break;		
			case sf::Event::MouseWheelMoved:
				cout << "Mouse wheel Scroll:" << event.mouseWheel.delta << endl;
				break;	
			case sf::Event::MouseMoved:
				// cout << "Mouse position: {" << event.mouseMove.x << ", " << event.mouseMove.y << "}" << endl;
				break;
			default:
				break;
		}	

		joystick.joystickActions(0);
		joystick.joystickActions(1);		
	}
}

// -------------------------------------------------------------------------------------------------

// When a mouse button is pressed (any button), this performs an action based on which button.
void EventHandler::mouseButton() {
	switch(event.key.code) {
		case sf::Mouse::Left:
			cout << "Mouse LEFT" << endl;
			makeNoise();
			break;
		case sf::Mouse::Right:
			cout << "Mouse RIGHT" << endl;
			break;
		case sf::Mouse::Middle:
			cout << "Mouse MIDDLE" << endl;
			break;
		case sf::Mouse::XButton1:
			cout << "Mouse XButton1" << endl;
			break;
		case sf::Mouse::XButton2:
			cout << "Mouse XButton2" << endl;
			break;
	}
}	

// -------------------------------------------------------------------------------------------------

void EventHandler::closeWindow() {
	cout << "Thanks for playing!" << endl;
	window->close();
}

// -------------------------------------------------------------------------------------------------

void EventHandler::resizeWindow() {
	screenWidth  = &event.size.width;
	screenHeight = &event.size.height;
	sf::FloatRect visibleArea(0, 0, *screenWidth, *screenHeight);
	window->setView(sf::View(visibleArea));
}

// -------------------------------------------------------------------------------------------------

void EventHandler::makeNoise() { // Sound effects test
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	cout << "Left mouse button pressed at {" << mousePosition.x << ", " << mousePosition.y << "}\n";
	if(mousePosition.x >= 50  && mousePosition.x <= 150 &&
		mousePosition.y >= 37 && mousePosition.y <= 183)
		gameSound->playSoundEffect("playerWin.ogg");
}