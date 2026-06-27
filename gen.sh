#!/bin/bash

BUILD_DIR="build"
OPTIONS="
-DCMAKE_EXPORT_COMPILE_COMMANDS=1
-DCMAKE_CXX_COMPILER_LAUNCHER=ccache
"

mkdir $BUILD_DIR;

cmake -S . -B $BUILD_DIR $OPTIONS -DCMAKE_BUILD_TYPE=Debug;

ln -sf $BUILD_DIR/compile_commands.json .;

REL_BUILD_DIR="build-rel"
cmake -S . -B $REL_BUILD_DIR $OPTIONS -DCMAKE_BUILD_TYPE=Release;
