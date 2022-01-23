# C/C++ Makefile generator

the program creates three folder named "src", "hdr", "bin" which respectively contain source(c or cpp), header(h or hpp) and bin(object and executeable) files.
Then it creates a main.c or main.cpp file in src folder with a boilerplate depend on the command line argument.
If the command line argument is gcc a C program enviroment will be created, if it is g++ a C++ program enviroment will be created. In other cases (neither gcc not g++ or no argument) it will create a gcc enviroment by default.
A makefile will be created which is capable of building and running project efficiently. It can also clean (remove) the files in bin folder
At the end the visual studio code will be opened with the current directory as workspace, so if you don't have vs code on your machine it will generate a enviroment correctly bu will crash at the end while trying to opne vs code.
This code is work on only Linux/Unix
