// Copyright Jonathan Carlson 2022

#include "miscellaneous.h"

vector<string> Miscellaneous::filterByFileType(vector<string> files, const string fileType) {

    for(short i = files.size() - 1; i > -1; i--) {
        if(!isFileType(files[i], fileType))
            files.erase(files.begin() + i);
    }

    return files;
}

/*------------------------------------------------------------------------------------------------*/

// Requires call 'srand(time(NULL))' in the main.cpp before any other initializations.
unsigned short Miscellaneous::generateRandomNumber(const short unsigned max) {
    return rand() % max + 1;
}

/*------------------------------------------------------------------------------------------------*/

vector<string> Miscellaneous::getAllFileNamesFromDirectory(const char *path) {

    struct dirent *entry;
    vector<string> files;
    DIR *directory = opendir(path);

    if (directory != NULL) {
        while((entry = readdir(directory)) != NULL) {
            string file = entry->d_name;

            if(file != "." && file != "..")
                files.push_back(file);
        }
    }

    closedir(directory);
    return files;
}

/*------------------------------------------------------------------------------------------------*/

vector<string> Miscellaneous::getFileTypesFromDirectory(const char *path, const string fileType) {
    vector<string> files = getAllFileNamesFromDirectory(path);
    return filterByFileType(files, fileType);
}

/*------------------------------------------------------------------------------------------------*/

bool Miscellaneous::isFileType(const string fileName, const string fileType) {
    std::size_t found = fileName.find(fileType);
    return found!=std::string::npos;
}

/*------------------------------------------------------------------------------------------------*/

float Miscellaneous::convertStringToFloat(string number) {
    std::string::size_type sz;
    return std::stof(number, &sz);
}   

/*------------------------------------------------------------------------------------------------*/

string Miscellaneous::shrinkWhiteSpacing(string str) {
    size_t found = str.find("  ");

    while(found!=string::npos) {
        str.erase(str.begin() + found);
        found = str.find("  ");
    }

    return str;
}

/*------------------------------------------------------------------------------------------------*/

vector<string> Miscellaneous::parseString(string str) {
    vector<string> words;
    stringstream line(str);
    string word;
    
    while(getline(line, word, ' ')) {
        words.push_back(word);
    }
    
    return words;            
}