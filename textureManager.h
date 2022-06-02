// Copyright Jonathan Carlson 2022

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <dirent.h> // directory
#include <cstring>
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

using namespace std;

class TextureManager {
public:

	TextureManager();

	map<string, sf::Texture> textures;

	string getTextureName(string fileName);

private:
	
	sf::Texture                  texture;
	vector<pair<string, string>> textureFilePaths;
	vector<string>  			 textureDirectories = {
		"Images/Objects",
		"Images/Backgrounds",
		"Images/Objects/PlayingCards",
		"Images/Objects/PlayingCards/100x150"
	};

	vector<string> getAllFileNamesFromDirectory(const char *path);
	void           loadTextureFilePathsFromDirectories();
	void 		   populateTextureList();
};


// =================================================================================================


TextureManager::TextureManager() {
	loadTextureFilePathsFromDirectories();
	populateTextureList();
}

// -------------------------------------------------------------------------------------------------

string TextureManager::getTextureName(string fileName) {
	size_t found = fileName.find('.');
	string textureName;

	if(found != string::npos)
		textureName = fileName.substr(0, found);

	return textureName;
}

// -------------------------------------------------------------------------------------------------

void TextureManager::populateTextureList() {
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

// -------------------------------------------------------------------------------------------------

void TextureManager::loadTextureFilePathsFromDirectories() {

	vector<string> supportedImageTypes = {".bmp", ".dds", ".jpg", ".png", ".tga", ".psd"};

	for(auto dir : textureDirectories) {

		char* cDir = new char[dir.length()];
		strcpy(cDir, dir.c_str()); 

		vector<string> textureFiles = getAllFileNamesFromDirectory(cDir);

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

// -------------------------------------------------------------------------------------------------

vector<string> TextureManager::getAllFileNamesFromDirectory(const char *path) {
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

#endif // TEXUTUREMANAGER_H