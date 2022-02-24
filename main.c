#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>
#include <getopt.h>

#ifdef __linux__
    #include <wait.h>
    #define MAKEFILE_TEMP1_PATH "/.genenv_config/makefile1"
    #define MAKEFILE_TEMP2_PATH "/.genenv_config/makefile2"
    #define MAIN_TEMP_PATH "/.genenv_config/main"
#endif

#ifdef __APPLE__
    #define MAKEFILE_TEMP1_PATH "/.genenv_config/template1"
    #define MAKEFILE_TEMP2_PATH "/.genenv_config/template2"
    #define MAIN_TEMP_PATH "/.genenv_config/main"
#endif


#define True 1
#define False 0

static char * HOME_PATH;

typedef unsigned char boolean;

void create_dir(char * dirName);
void create_dirs(boolean is_advanced);
void create_makefile(boolean is_C, boolean is_advanced);
void create_main(boolean is_C);

int main(int argc, char ** argv)
{
    HOME_PATH = getenv("HOME");

    boolean is_c = True;
    boolean run_vs_code = False;
    boolean advanced = False;

    int opt;

    while ((opt = getopt(argc, argv, "c:vad:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            if(strcmp(optarg, "g++") == 0)
            {
                is_c = False;
            }
            else if(strcmp(optarg, "gcc") != 0)
            {
                printf("Unknown compiler entered, so gcc will be used\n");
            }
            break;
        case 'v':
            run_vs_code = True;
            break;
        case 'a':
            advanced = True;
            break;
        case 'd':
            create_dir(optarg);
            chdir(optarg);
            break;
        default:
            break;
        }
    }
    create_dirs(advanced);
    create_makefile(is_c, advanced);
    create_main(is_c);

    if(run_vs_code)
        execlp("code", "code", ".", NULL);

    return 0;
}

void create_main(boolean is_C)
{
    char * fileName;
    char * heading;

    if (is_C)
    {
        fileName = "src/main.c";
        heading = "#include <stdio.h>\n#include <stdlib.h>\n\n";
    }
    else
    {
        fileName = "src/main.cpp";
        heading = "#include <iostream>\n\nusing namespace std;\n\n";
    }


    int mainFile = open(
            fileName, O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
        );
    
    write(mainFile, heading, strlen(heading));

    char mainPath[255]; strcpy(mainPath, HOME_PATH);
    strcat(mainPath, MAIN_TEMP_PATH);

    int template = open(mainPath, O_RDONLY);

    char letter;
    while(read(template, &letter, 1) > 0)
        write(mainFile, &letter, 1);

    close(mainFile);
    close(template);
}

void create_makefile(boolean is_C, boolean is_advanced)
{
    int Makefile = open(
        "Makefile", O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
    );

    if(Makefile == -1){ printf("Error while creating Makefile\n"); exit(1); }

    if (is_C)
    {
        write(Makefile, "COMPILER = gcc\nEX = .c\n\n", 24);
    }
    else
    {
        write(Makefile, "COMPILER = g++\nEX = .cpp\n\n", 26);
    }
    
    int template;
    char makefilePath[255]; strcpy(makefilePath, HOME_PATH);

    if (is_advanced)
    {
        strcat(makefilePath, MAKEFILE_TEMP2_PATH);
        template = open(makefilePath, O_RDONLY);
    }
    else
    {
        strcat(makefilePath, MAKEFILE_TEMP1_PATH);
        template = open(makefilePath, O_RDONLY);
    }

    char letter;
    while(read(template, &letter, 1) > 0)
        write(Makefile, &letter, 1);
    
    close(template);
    close(Makefile);
}

void create_dir(char * dirName)
{
    pid_t pid = fork();
    if(pid == 0)
    {
        execlp("mkdir", "mkdir", dirName, NULL);
        exit(1);
    }
    wait(NULL);
}


void create_dirs(boolean is_advanced)
{
    create_dir("src");
    create_dir("bin");
    create_dir("include");
    
    if(is_advanced)
    {
        create_dir("bin/debug");
        create_dir("bin/release");
    }
}
