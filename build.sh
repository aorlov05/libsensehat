#!/bin/bash

BUILD_DIR=build
INSTALL_DIR=~

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR ..
cmake --build .
cmake --install .

cd ..
echo "Installed headers to ~/include/sensehat and library to ~/lib/sensehat"
