#!/usr/bin/env bash

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

cd build
cmake ..
make

make install

if [[ $1 == --clean ]]; then
    echo "Cleaning build folder"
    make clean
    rm -rf *
fi

cd ../