#!/bin/sh
ROOT=$(pwd)

cd dep
tar xf gmp-6.0.0a.tar.xz
cd gmp-6.0.0
./configure --prefix=$ROOT
make
make install
cd ../..

mkdir build
g++ -I include -L lib -l gmp -o build/exercise_1.exe program/exercise_1.cpp
chmod +x build/exercise_1.exe