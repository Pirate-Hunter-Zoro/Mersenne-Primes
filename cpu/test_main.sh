#!/bin/bash

cd "$(dirname "$0")"
mkdir -p build
g++ -std=c++17 -o build/main main.cpp

for p in 2 3 4 5 6 7 8 11 13 17 19; do
    build/main "$p"
done