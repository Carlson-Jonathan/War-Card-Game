#!/bin/bash
clear

# For normal compiling.
g++ -c main.cpp miscellaneous.cpp
g++ main.o miscellaneous.o -o executable.out -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# --------------------------------------------------------------------------------------------------

# For cross-compiling (generates .exe for Windows). 
# Make sure your SFML .dll files are in your compile directory!
# x86_64-w64-mingw32-g++ main.cpp miscellaneous.cpp -o main.exe -I../SFML-2.5.1/include -L../SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

exit
