#!/bin/sh
mkdir bin
gcc main.c -o bin/genenv -O3
mkdir ~/.genenv_config
cp res/* ~/.genenv_config/
sudo cp bin/genenv /usr/local/bin/genenv