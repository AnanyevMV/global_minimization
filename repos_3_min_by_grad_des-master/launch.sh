#!/bin/bash

if [[ $1 = "dev" ]]; then
    docker exec cmake_project ./build.sh dev
fi

if [[ $1 = "build" ]]; then
    docker exec cmake_project ./build.sh build
fi
