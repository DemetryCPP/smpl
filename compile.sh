#!/bin/sh

CC="g++-11"
FLAGS="-o smpl -Iinclude/ -std=c++2a"
FILES="src/lexer.cpp
       src/parser.cpp
       src/utils.cpp"

$CC $FLAGS $FILES $1