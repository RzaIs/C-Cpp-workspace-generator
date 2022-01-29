# C/C++ Makefile generator

the program creates three folder named "src", "hdr", "bin" which respectively contain source(c or cpp), header(h or hpp) and bin(object and executeable) files.(with the flag -a it creates "debug" and "release" folder separately in bin)
Then it creates a main.c or main.cpp file in src folder with a boilerplate depend on the command line argument.
If the command line argument is "-c g++" a C++ program enviroment will be created, if it is "-c gcc" a C program enviroment will be created. In other cases (neither gcc not g++ or no argument) it will create a gcc enviroment by default.
A makefile will be created which is capable of building and running project efficiently.if "-a" flag has been entered then it will have advanced options like debug and release build It can also clean (remove) the files in bin folder
At the end the visual studio code will be opened if "-v" is given with the current directory as workspace, so if you don't have vs code on your machine it will generate a enviroment correctly bu will crash at the end while trying to opne vs code if "-v" is given.
This code works on only Linux/Unix
