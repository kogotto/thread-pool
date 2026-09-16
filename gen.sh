#!/bin/bash

source defs.sh

cmake -S . -B $BUILD_DIR $OPTIONS -DCMAKE_BUILD_TYPE=Debug;
ln -sf $BUILD_DIR/compile_commands.json .;

cmake -S . -B $REL_BUILD_DIR $OPTIONS -DCMAKE_BUILD_TYPE=Release;

cmake -S . -B $TSAN_BUILD_DIR $OPTIONS -DCMAKE_BUILD_TYPE=TSAN;
