// Copyright Jonathan Carlson 2022

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
using namespace std;

#include <dirent.h>     // directory
#include <cstring>
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

class TextureManager {
public:

	TextureManager() {
		loadTextureFilePathsFromDirectories();
		populateTextureList();
	}

	map<string, sf::Texture> textures;

    //----------------------------------------------------------------------------------------------

	string getTextureName(string fileName) {
		size_t found = fileName.find('.');
		string textureName;

    	if(found != string::npos)
			textureName = fileName.substr(0, found);

		return textureName;
	}

    //----------------------------------------------------------------------------------------------

private:
	
	// All paths to directories containing images you wish to load go here (relative to this file).
	vector<string> textureDirectories = {
		"Images/Backgrounds",
		"Images/Objects/PlayingCards/100x150"
	};

	sf::Texture texture;
	vector< pair<string, string> > textureFilePaths;
	
	//----------------------------------------------------------------------------------------------

	// File names will not include the directory path or the file's extension.
	// Example: 'dir/somefile.jpg' will be 'somefile'
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

	void loadTextureFilePathsFromDirectories() {

		vector<string> supportedImageTypes = {".bmp", ".dds", ".jpg", ".png", ".tga", ".psd"};

		for(auto dir : textureDirectories) {

			char cDir[dir.length()];
			strcpy(cDir, dir.c_str()); 

			vector textureFiles = getAllFileNamesFromDirectory(cDir);

			for(auto file : textureFiles) {

				bool invalidFile = true;

				for(auto type : supportedImageTypes) {
					size_t found = file.find(type);
					if(found != string::npos) {
						invalidFile = false;
					}
				}

				if(invalidFile)
					continue;

				textureFilePaths.push_back({getTextureName(file), (dir + "/" + file)});
			}
		}
	}

	//----------------------------------------------------------------------------------------------

	// Loads a map<string, sf::texture> with the texture name and sprite texture.
	void populateTextureList() {
		for(int i = 0; i < textureFilePaths.size(); i++) {

			if(!texture.loadFromFile(textureFilePaths[i].second)) {
				cout << "ERROR: TextureManager::populateTextures(): Unable to load image '" 
					 << textureFilePaths[i].second << "'" << endl;
				exit(139);
			}
			else {
				textures.insert({textureFilePaths[i].first, texture});
			}
		}
	}
};

#endif // TEXUTUREMANAGER_H