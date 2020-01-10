#!/bin/bash

# Installation directory can be overridden, defaults to ~/.local/bin
export INSTALL_DIR=~/.local/bin

cmake -B cmake-build-release -D CMAKE_BUILD_TYPE=Release
cd cmake-build-release
mkdir -p $INSTALL_DIR
make && make install
