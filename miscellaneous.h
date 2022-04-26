// Copyright Jonathan Carlson 2022

#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

#include <iostream>
using namespace std;

#include <dirent.h>     // directory
#include <vector>
#include <sstream>
#include <stdlib.h>     // srand, rand 
#include <time.h>       

// A library for miscellaneous, non-cohesive functions that do not need initialization.

class Miscellaneous {

public:
    static vector<string> filterByFileType(vector<string> files, const string fileType);
    static unsigned short generateRandomNumber(const short unsigned max);
    static vector<string> getFileTypesFromDirectory(const char *path, const string fileType);
    static vector<string> getAllFileNamesFromDirectory(const char *path);
    static bool           isFileType(const string fileName, const string fileType);
    static float          convertStringToFloat(string number);
    static string         shrinkWhiteSpacing(string str);
    static vector<string> parseString(string str);
};

#endif // MISCELLANEOUS_H