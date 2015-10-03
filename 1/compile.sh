#!/bin/sh
mkdir build
g++ -I include -L lib -l gmp -o build/exercise_1.exe program/exercise_1.cpp
chmod +x build/exercise_1.exe