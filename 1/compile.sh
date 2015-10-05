#!/bin/sh
mkdir build
g++ program/exercise_1.cpp -I include -L lib -l gmpxx -l gmp -o build/exercise_1.exe -g
chmod +x build/exercise_1.exe
