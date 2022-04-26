// Copyright Jonathan Carlson 2022

#ifndef MISCELLANEOUS_TEST_H
#define MISCELLANEOUS_TEST_H

#include <iostream>
#include "../miscellaneous.h"
#include <set>
#include <typeinfo>
#include <vector>
#include <time.h>
using namespace std;

class Miscellaneous_Test {
public:

	Miscellaneous_Test() {}

	static bool generateRandomNumber_test();
	static bool convertStringToFloat_test();

};
#endif // MISCELLANEOUS_H