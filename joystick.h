#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <iostream>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace std;

class Joystick {
public: 

    Joystick() {}

    void joystickActions(short joystickNumber);
};


// =================================================================================================


void Joystick::joystickActions(short joystickNumber) {

    vector<bool> joystickButtons;
    for(short i = 0; i < 18; i++) {
        joystickButtons.push_back(sf::Joystick::isButtonPressed(joystickNumber, i));
    };

    for(short i = 0; i < joystickButtons.size(); i++) {
        if(joystickButtons[i]) {
            cout << "Joystick " << joystickNumber << " Button " << i << " is pressed " << endl;
        }
    }
}

#endif // JOYSTICK