#!/bin/bash
clear
g++ -c testHarness.cpp miscellaneous_test.cpp ../miscellaneous.cpp
g++ testHarness.o miscellaneous.o miscellaneous_test.o -o executable.out -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
./executable.out
exit