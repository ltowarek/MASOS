#!/bin/sh
mkdir build
g++ program/exercise_1.cpp -I include -L lib -l gmp -o build/exercise_1.exe
chmod +x build/exercise_1.exe