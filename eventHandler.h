// Copyright Jonathan Carlson 2022

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <iostream>
using namespace std;

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/* 
	It works, but it obviously needs a major refactor/cleanup
*/

class EventHandler {
public:

	EventHandler() {}
	EventHandler(sf::RenderWindow & window, unsigned int & screenWidth, unsigned int & screenHeight) {
		this->window = &window;
		this->screenWidth = &screenWidth;
		this->screenHeight = &screenHeight;
	}

	bool Joystick_0_Button_0  = false;
	bool Joystick_0_Button_1  = false;
	bool Joystick_0_Button_2  = false;
	bool Joystick_0_Button_3  = false;
	bool Joystick_0_Button_4  = false;
	bool Joystick_0_Button_5  = false;
	bool Joystick_0_Button_6  = false;
	bool Joystick_0_Button_7  = false;
	bool Joystick_0_Button_8  = false;
	bool Joystick_0_Button_9  = false;
	bool Joystick_0_Button_10 = false;
	bool Joystick_0_Button_11 = false;
	bool Joystick_0_Button_12 = false;
	bool Joystick_0_Button_13 = false;
	bool Joystick_0_Button_14 = false;
	bool Joystick_0_Button_15 = false;
	bool Joystick_0_Button_16 = false;
	bool Joystick_0_Button_17 = false;
	bool Joystick_0_Button_18 = false;
	bool Joystick_0_Button_19 = false;
	bool Joystick_0_Button_20 = false;
	bool Joystick_0_Button_21 = false;
	bool Joystick_0_Button_22 = false;
	bool Joystick_0_Button_23 = false;

	/*------------------------------------------------------------------------------------------------*/

	void listen() {
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
					setAllButtonsToDefault();
					break;
			}	

			joystick(0);
			joystick(1);		
			setJoystickButtonPresses();
	    }
	}

private:

	unsigned int* screenWidth;
	unsigned int* screenHeight;

	sf::Event event;
	sf::RenderWindow* window;

	/*------------------------------------------------------------------------------------------------*/

	void closeWindow() {
		cout << "Thanks for playing!" << endl;
		window->close();
	}

	/*------------------------------------------------------------------------------------------------*/

	void resizeWindow() {
		screenWidth  = &event.size.width;
	    screenHeight = &event.size.height;
	    sf::FloatRect visibleArea(0, 0, *screenWidth, *screenHeight);
	    window->setView(sf::View(visibleArea));
	}

	/*------------------------------------------------------------------------------------------------*/

	void mouseButton() {
		switch(event.key.code) {
			case sf::Mouse::Left:
				cout << "Mouse LEFT" << endl;
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

	/*------------------------------------------------------------------------------------------------*/

	void joystick(short joystickNum) {
		vector<bool> Joystick_Buttons = {
			sf::Joystick::isButtonPressed(joystickNum, 0),			
			sf::Joystick::isButtonPressed(joystickNum, 1),
			sf::Joystick::isButtonPressed(joystickNum, 2),
			sf::Joystick::isButtonPressed(joystickNum, 3),
			sf::Joystick::isButtonPressed(joystickNum, 4),
			sf::Joystick::isButtonPressed(joystickNum, 5),
			sf::Joystick::isButtonPressed(joystickNum, 6),
			sf::Joystick::isButtonPressed(joystickNum, 7),
			sf::Joystick::isButtonPressed(joystickNum, 8),
			sf::Joystick::isButtonPressed(joystickNum, 9),
			sf::Joystick::isButtonPressed(joystickNum, 10),
			sf::Joystick::isButtonPressed(joystickNum, 11),
			sf::Joystick::isButtonPressed(joystickNum, 12),
			sf::Joystick::isButtonPressed(joystickNum, 13),
			sf::Joystick::isButtonPressed(joystickNum, 14),
			sf::Joystick::isButtonPressed(joystickNum, 15),
			sf::Joystick::isButtonPressed(joystickNum, 16),
			sf::Joystick::isButtonPressed(joystickNum, 17),
			sf::Joystick::isButtonPressed(joystickNum, 18)									
		};

		for(short i = 0; i < Joystick_Buttons.size(); i++) {
			if(Joystick_Buttons[i]) {
				cout << "Joystick " << joystickNum << " Button" << i << endl;
			}
		}

		// cout << "Joystick Axis: {" << sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::X) << ", "
		// 	 << sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Y) << "}" << endl;

	}

	/*------------------------------------------------------------------------------------------------*/

	void setJoystickButtonPresses() {
			Joystick_0_Button_0  = sf::Joystick::isButtonPressed(0, 0);			
			Joystick_0_Button_1  = sf::Joystick::isButtonPressed(0, 1);
			Joystick_0_Button_2  = sf::Joystick::isButtonPressed(0, 2);
			Joystick_0_Button_3  = sf::Joystick::isButtonPressed(0, 3);
			Joystick_0_Button_4  = sf::Joystick::isButtonPressed(0, 4);
			Joystick_0_Button_5  = sf::Joystick::isButtonPressed(0, 5);
			Joystick_0_Button_6  = sf::Joystick::isButtonPressed(0, 6);
			Joystick_0_Button_7  = sf::Joystick::isButtonPressed(0, 7);
			Joystick_0_Button_8  = sf::Joystick::isButtonPressed(0, 8);
			Joystick_0_Button_9  = sf::Joystick::isButtonPressed(0, 9);
			Joystick_0_Button_10 = sf::Joystick::isButtonPressed(0, 10);
			Joystick_0_Button_11 = sf::Joystick::isButtonPressed(0, 11);
			Joystick_0_Button_12 = sf::Joystick::isButtonPressed(0, 12);
			Joystick_0_Button_13 = sf::Joystick::isButtonPressed(0, 13);
			Joystick_0_Button_14 = sf::Joystick::isButtonPressed(0, 14);
			Joystick_0_Button_15 = sf::Joystick::isButtonPressed(0, 15);
			Joystick_0_Button_16 = sf::Joystick::isButtonPressed(0, 16);
			Joystick_0_Button_17 = sf::Joystick::isButtonPressed(0, 17);
			Joystick_0_Button_18 = sf::Joystick::isButtonPressed(0, 18);
	}

    //----------------------------------------------------------------------------------------------

	void setAllButtonsToDefault() {
		Joystick_0_Button_0  = false;
		Joystick_0_Button_1  = false;
		Joystick_0_Button_2  = false;
		Joystick_0_Button_3  = false;
		Joystick_0_Button_4  = false;
		Joystick_0_Button_5  = false;
		Joystick_0_Button_6  = false;
		Joystick_0_Button_7  = false;
		Joystick_0_Button_8  = false;
		Joystick_0_Button_9  = false;
		Joystick_0_Button_10 = false;
		Joystick_0_Button_11 = false;
		Joystick_0_Button_12 = false;
		Joystick_0_Button_13 = false;
		Joystick_0_Button_14 = false;
		Joystick_0_Button_15 = false;
		Joystick_0_Button_16 = false;
		Joystick_0_Button_17 = false;
		Joystick_0_Button_18 = false;
	}

    //----------------------------------------------------------------------------------------------

	void printJoyButtons() {
		cout << "J0_B0 = "  << Joystick_0_Button_0  << endl;
		cout << "J0_B1 = "  << Joystick_0_Button_1  << endl;
		cout << "J0_B2 = "  << Joystick_0_Button_2  << endl;
		cout << "J0_B3 = "  << Joystick_0_Button_3  << endl;
		cout << "J0_B4 = "  << Joystick_0_Button_4  << endl;
		cout << "J0_B5 = "  << Joystick_0_Button_5  << endl;
		cout << "J0_B6 = "  << Joystick_0_Button_6  << endl;
		cout << "J0_B7 = "  << Joystick_0_Button_7  << endl;
		cout << "J0_B8 = "  << Joystick_0_Button_8  << endl;
		cout << "J0_B9 = "  << Joystick_0_Button_9  << endl;
		cout << "J0_B10 = " << Joystick_0_Button_10 << endl;
		cout << "J0_B11 = " << Joystick_0_Button_11 << endl;
		cout << "J0_B12 = " << Joystick_0_Button_12 << endl;
		cout << "J0_B13 = " << Joystick_0_Button_13 << endl;
		cout << "J0_B14 = " << Joystick_0_Button_14 << endl;
		cout << "J0_B15 = " << Joystick_0_Button_15 << endl;
	}
};
#endif // EVENTHANDLER_H