#!/bin/bash
clear
g++ -c main.cpp miscellaneous.cpp
g++ main.o miscellaneous.o -o executable.out -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
./executable.out
rm main.o
exit