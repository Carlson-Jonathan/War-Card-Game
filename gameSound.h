// Copyright Jonathan Carlson 2022

#ifndef GAMESOUND_H
#define GAMESOUND_H

#include <iostream>
#include <map>
#include <dirent.h>     // directory
#include <cstring>
using namespace std;

#include <SFML/Audio.hpp>

class GameSound {
public:

	GameSound() {
		SFMLmusic.setLoop(true);
		SFMLmusic.setVolume(35.f);
		loadSoundFilePathsFromDirectories();
		loadSoundEffects();

		for(auto i : soundFilePaths) { 
			cout << i.first << " | " << i.second << endl;
		}
	}

	sf::SoundBuffer buffer;
	sf::Sound soundEffect;

	vector<string> getAllFileNamesFromDirectory(const char *path) {
    	struct dirent *entry;
    	vector<string> files;
    	DIR *directory = opendir(path);

    	if (directory != NULL) {
	        while((entry = readdir(directory)) != NULL) {
            	string file = entry->d_name;
	                files.push_back(file);
        	}
    	}

	    closedir(directory);
	    return files;
	}

    //----------------------------------------------------------------------------------------------

	void loadSoundFilePathsFromDirectories() {

		vector<string> supportedSoundTypes = {".wav", ".ogg"};

		for(auto dir : soundDirectories) {
			char cDir[dir.length()];
			strcpy(cDir, dir.c_str()); 

			vector soundFiles = getAllFileNamesFromDirectory(cDir);

			for(auto file : soundFiles) {
				bool invalidFile = true;

				for(auto type : supportedSoundTypes) {
					size_t found = file.find(type);
					if(found != string::npos) {
						invalidFile = false;
					}
				}

				if(invalidFile)
					continue;

				soundFilePaths.push_back({file, (dir + "/" + file)});
			}
		}
	}

    //----------------------------------------------------------------------------------------------

	void loadSoundEffects() {

		for(short i = 0; i < soundFilePaths.size(); i++) {

			sf::Sound soundEffect;
			soundEffectBuffers.push_back(new sf::SoundBuffer);

			if(!(*soundEffectBuffers[i]).loadFromFile(soundFilePaths[i].second)) {
				cout << "ERROR: Unable to load sound effect " << endl;
				exit(139);
			}

			soundEffect.setBuffer(*soundEffectBuffers[i]);
			soundEffects.insert({soundFilePaths[i].first, soundEffect});
		}
	}

    //----------------------------------------------------------------------------------------------

	void playSoundEffect(string effect) {
		soundEffects[effect].play();
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
	vector<sf::SoundBuffer*> soundEffectBuffers;
	map<string, sf::Sound> soundEffects;

	vector<pair<string, string>> soundFilePaths;
	vector<string> soundDirectories = {
		"Sounds/Effects",
	};

};

#endif // GAMESOUND_H