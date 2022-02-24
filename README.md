# C/C++ Worksace generator

## What this program does?
The program creates directories "src" "include" and "bin" for orgaising your workspace and also creates a makefile for easy compile/run.

## How to install
Simple! Open a terminal in the project folder and type "sh install.sh" and enter your password for root access (this is required to move executable program inside "/usr/local/bin")

## How to use it?
With the "genenv" command. There is some flags you should know how to use. '-c' flag is to determine your compiler is either C or C++ compiler.Use it like "-c gcc" or "-c g++". the default compiler is gcc.
'-a' command creates two folder in "bin" "debug" and "release" and creates makefile repectively, which let's you to compile and run your code more specifically. '-d' flag is to choose your workspace if you don't want to use current directory as your workspace. Use it like " -d 'your/work/space' ". "-v" opens vs code on your worksapce if you want. As an example if we want to create a C++ workspace in a custom folder with advanced build management and open cs vode it will be "genenv -c g++ -d project1 -a -v".

## Platforms
This code works on Unix/Linux systems.