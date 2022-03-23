#!/bin/sh
mkdir bin || echo "bin already exist"
gcc main.c -o bin/genenv -O3
rm ~/.genenv_config/*
cp res/* ~/.genenv_config/
sudo rm /usr/local/bin/genenv
sudo cp bin/genenv /usr/local/bin/genenv