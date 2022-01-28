#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>
#include <wait.h>

#define True 1
#define False 0

typedef unsigned char boolean;

void error_check(int result, char * dirName);
void create_dirs();
void create_makefile(boolean is_C);
void create_main(boolean is_C);

int main(int argc, char const *argv[])
{
    create_dirs();

    if (argc < 2)
    {
        printf("No compiler name provided, so gcc will be used\n");
        create_makefile(True);
        create_main(True);
    }
    else
    {
        if (strcmp(argv[1], "g++") == 0)
        {
            create_makefile(False);
            create_main(False);
        }
        else if (strcmp(argv[1], "gcc") == 0)
        {
            create_makefile(True);
            create_main(True);
        }
        else
        {
            printf("Unknown compiler name provided, so gcc will be used\n");
            create_makefile(True);
            create_main(True);
        }
        
    }

    execlp("code", "code", ".", NULL);
    return 0;
}

void create_main(boolean is_C)
{
    int fd;
    if(is_C)
    {
        fd = open(
            "src/main.c", O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
        );

        char * content = \
"#include <stdio.h>\n\
#include <stdlib.h>\n\n\
int main(int argc, char const *argv[])\n\
{\n\
\t/* code */\n\
\treturn 0;\n\
}\n";

        write(fd, content, 106);

    }
    else
    {
        fd = open(
            "src/main.cpp", O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
        );

        char * content = \
"#include <iostream>\n\n\
using namespace std;\n\n\
int main(int argc, char const *argv[])\n\
{\n\
\t/* code */\n\
\treturn 0;\n\
}\n";

        write(fd, content, 109);

    }
    close(fd);
}

void create_makefile(boolean is_C)
{
    int fd = open(
        "Makefile", O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
    );

    if(fd == -1){ printf("Error while creating Makefile\n"); exit(1); }

    if (is_C)
    {
        write(fd, "COMPILER = gcc\nEX = .c\n", 23);
    }
    else
    {
        write(fd, "COMPILER = g++\nEX = .cpp\n", 25);
    }

    char * content = \
"OBJdir = bin\n\
HDRdir = hdr\n\
SRCdir = src\n\n\
obj = $(patsubst $(SRCdir)/%$(EX), $(OBJdir)/%.o, $(wildcard $(SRCdir)/*))\n\
hdr = $(wildcard $(HDRdir)/*)\n\n\
build: $(OBJdir)/run\n\n\
run: $(OBJdir)/run\n\t./$^\n\n\
clean:\n\trm $(OBJdir)/*\n\n\
$(OBJdir)/run: $(obj)\n\t$(COMPILER) $^ -o $@\n\n\
$(OBJdir)/%.o: $(SRCdir)/%$(EX) $(hdr)\n\t\
$(COMPILER) -c $< -o $@ -I \"$(HDRdir)/\"\n";

    write(fd, content, 343);
    close(fd);
}


void create_dirs()
{
    errno = 0;
    
    int result;
    pid_t pid;

    result = mkdir("hdr", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    pid = fork();
    if(pid == 0)
    {
        error_check(result, "hdr");
        exit(1);
    }
    wait(NULL);

    result = mkdir("src", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    pid = fork();
    if(pid == 0)
    {
        error_check(result, "src");
        exit(1);
    }
    wait(NULL);

    result = mkdir("bin", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    pid = fork();
    if(pid == 0)
    {
        error_check(result, "bin");
        exit(1);
    }
    wait(NULL);
}

void error_check(int result, char * dirName)
{
    if (result == -1) {
        printf("for %s: ", dirName);
        switch (errno) {
            case EACCES :
                printf("the parent directory does not allow write\n");
                exit(EXIT_FAILURE);
            case EEXIST:
                printf("pathname already exists\n");
                exit(EXIT_FAILURE);
            case ENAMETOOLONG:
                printf("pathname is too long\n");
                exit(EXIT_FAILURE);
            default:
                perror("mkdir\n");
                exit(EXIT_FAILURE);
        }
    }
}
