#!/bin/bash

CC="g++-11"
FLAGS="-Isrc/headers -o smpl -std=c++2a"
FILES="src/lexer.cpp
       src/parser.cpp
       src/smpl.cpp
       src/token.cpp"

$CC $FLAGS $FILES $1