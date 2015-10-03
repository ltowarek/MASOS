#!/bin/sh
ROOT=$(pwd)

cd dep
tar xf gmp-6.0.0a.tar.xz
cd gmp-6.0.0
./configure --prefix=$ROOT
make
make install
cd ../..

chmod +x compile.sh
./compile.sh