#!/bin/bash

BUILD_DIR="build"
OPTIONS="
-DCMAKE_EXPORT_COMPILE_COMMANDS=1
-DCMAKE_CXX_COMPILER_LAUNCHER=ccache
-DCMAKE_BUILD_TYPE=Debug
"

mkdir $BUILD_DIR;

cmake -S . -B $BUILD_DIR $OPTIONS;

ln -sf $BUILD_DIR/compile_commands.json .;
