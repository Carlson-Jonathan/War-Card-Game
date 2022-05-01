#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <iostream>
using namespace std;

#include <vector>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Joystick {
public: 

    Joystick() {}

    vector<bool> joystickButtonStates;

    // ---------------------------------------------------------------------------------------------

	void joystickActions(short joystickNum) {

		vector<bool> joystickButtons;
		for(short i = 0; i < 18; i++) {
			joystickButtons.push_back(sf::Joystick::isButtonPressed(joystickNum, i));
		};

		for(short i = 0; i < joystickButtons.size(); i++) {
			if(joystickButtons[i]) {
				cout << "Joystick " << joystickNum << " Button " << i << " is pressed " << endl;
			}
		}

		// cout << "Joystick Axis: {" << sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::X) << ", "
		// 	 << sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Y) << "}" << endl;
	}
};

#endif // JOYSTICK