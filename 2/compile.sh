#!/bin/sh
mkdir build
g++ program/exercise_2.cpp -I ../include -L ../lib -l gmpxx -l gmp -o build/exercise_2.exe -g
chmod +x build/exercise_2.exe
