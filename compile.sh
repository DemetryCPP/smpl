#!/bin/bash

CC="g++"
FLAGS="-Isrc/headers -o smpl"
FILES="src/lexer.cpp
       src/parser.cpp
       src/smpl.cpp
       src/token.cpp"

$CC $FLAGS $FILES $1