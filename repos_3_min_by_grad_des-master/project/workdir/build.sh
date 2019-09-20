#!/bin/bash


if [[ $1 = "dev" ]]; then
    cmake -H. -Bbuild
    cmake --build build -- -j3
    cd build/ && ./test
    cd .. && rm -rf build/
fi


if [[ $1 = "build" ]]; then
    cmake -H. -Bbuild
    cmake --build build -- -j3
fi

if [[ $1 = "null" ]]; then
    rm -rf build/
    rm -rf json/
fi
