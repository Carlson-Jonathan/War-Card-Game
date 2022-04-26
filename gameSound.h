// Copyright Jonathan Carlson 2022

#ifndef GAMESOUND_H
#define GAMESOUND_H

#include <iostream>
using namespace std;

#include <SFML/Audio.hpp>

class GameSound {
public:

	GameSound() {
		SFMLmusic.setLoop(true);
		SFMLmusic.setVolume(35.f);
	}

    //----------------------------------------------------------------------------------------------

	void loadAndPlayMusic(string song) {
		if(!SFMLmusic.openFromFile(song)) {
			cout << "ERROR: GameSound::loadAndPlayMusic(): Unable to load file '" << song << "'" << endl;
			exit(139);
		}
		SFMLmusic.play();
	}

    //----------------------------------------------------------------------------------------------

	void loadAndPlayRandomSongFromDirectory() {}

private: 

	sf::Music SFMLmusic;

};

#endif // GAMESOUND_H