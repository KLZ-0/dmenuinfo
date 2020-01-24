#!/bin/bash

# Installation directory can be overridden, defaults to ~/.local/bin
export INSTALL_DIR=~/.local/bin

tmp=$( realpath "$0"  )
PROJECT_PATH=$(dirname "$tmp")

cd $PROJECT_PATH
cmake -B cmake-build-release -D CMAKE_BUILD_TYPE=Release
cd cmake-build-release
mkdir -p $INSTALL_DIR
make && make install
