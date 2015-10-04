#!/bin/sh
ROOT=$(pwd)
wget https://gmplib.org/download/gmp/gmp-6.0.0a.tar.xz
tar xf gmp-6.0.0a.tar.xz
cd gmp-6.0.0 && ./configure --prefix=$ROOT && make && make install
