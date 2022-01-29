#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>

#ifdef __linux__
    #include <wait.h>
#endif
#include <getopt.h>

#define True 1
#define False 0

typedef unsigned char boolean;

void create_dirs(boolean is_advanced);
void create_makefile(boolean is_C, boolean is_advanced);
void create_main(boolean is_C);

int main(int argc, char * const * argv)
{
    boolean is_c = True;
    boolean run_vs_code = False;
    boolean advanced = False;

    int opt;

    while ((opt = getopt(argc, argv, "c:va")) != -1)
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

void create_makefile(boolean is_C, boolean is_advanced)
{
    int fd = open(
        "Makefile", O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
    );

    if(fd == -1){ printf("Error while creating Makefile\n"); exit(1); }

    if (is_C)
    {
        write(fd, "COMPILER = gcc\nEX = .c\n\n", 24);
    }
    else
    {
        write(fd, "COMPILER = g++\nEX = .cpp\n\n", 26);
    }

    if(is_advanced)
    {
        char * content = \
"OBJdir = bin\n\
DEBUGdir = $(OBJdir)/debug\n\
RELEASEdir = $(OBJdir)/release\n\
HDRdir = hdr\n\
SRCdir = src\n\n\
DEBUGflags = -m64 -g -Wall\n\
RELEASEflags = -m64 -O3 -Wall\n\n\
LINKERS =\n\n\
debug_obj = $(patsubst $(SRCdir)/%$(EX), $(DEBUGdir)/%.o, $(wildcard $(SRCdir)/*))\n\
release_obj = $(patsubst $(SRCdir)/%$(EX), $(RELEASEdir)/%.o, $(wildcard $(SRCdir)/*))\n\
hdr = $(wildcard $(HDRdir)/*)\n\n\
build_debug: $(DEBUGdir)/run\n\
build_release: $(RELEASEdir)/run\n\n\
run_debug: $(DEBUGdir)/run\n\t./$^\n\n\
run_release: $(RELEASEdir)/run\n\t./$^\n\n\
clean:\n\
\t@if [ -f $(DEBUGdir)/*.o ]; then rm $(DEBUGdir)/*.o; fi\n\
\t@if [ -f $(DEBUGdir)/run ]; then rm $(DEBUGdir)/run; fi\n\
\t@if [ -f $(RELEASEdir)/*.o ]; then rm $(RELEASEdir)/*.o; fi\n\
\t@if [ -f $(RELEASEdir)/run ]; then rm $(RELEASEdir)/run; fi\n\
\n\
$(DEBUGdir)/run: $(debug_obj)\n\
\t$(COMPILER) $^ -o $@ \\\n\
\t$(LINKERS)\n\
\n\
$(RELEASEdir)/run: $(release_obj)\n\
\t$(COMPILER) $^ -o $@ \\\n\
\t$(LINKERS)\n\
\n\
$(DEBUGdir)/%.o: $(SRCdir)/%$(EX) $(hdr)\n\
\t$(COMPILER) -c $< -o $@ \\\n\
\t-I \"$(HDRdir)/\" $(DEBUGflags)\n\
\n\
$(RELEASEdir)/%.o: $(SRCdir)/%$(EX) $(hdr)\n\
\t$(COMPILER) -c $< -o $@ \\\n\
\t-I \"$(HDRdir)/\" $(RELEASEflags)\n";

        write(fd, content, 1088);
    }
    else
    {
        char * content = \
"OBJdir = bin\n\
HDRdir = hdr\n\
SRCdir = src\n\n\
obj = $(patsubst $(SRCdir)/%$(EX), $(OBJdir)/%.o, $(wildcard $(SRCdir)/*))\n\
hdr = $(wildcard $(HDRdir)/*)\n\n\
build: $(OBJdir)/run\n\n\
run: $(OBJdir)/run\n\t./$^\n\n\
clean:\n\
\t@if [ -f $(OBJdir)/* ]; then rm $(OBJdir)/*; fi\n\n\
$(OBJdir)/run: $(obj)\n\t$(COMPILER) $^ -o $@\n\n\
$(OBJdir)/%.o: $(SRCdir)/%$(EX) $(hdr)\n\t\
$(COMPILER) -c $< -o $@ -I \"$(HDRdir)/\"\n";

        write(fd, content, 376);
    }
    
    close(fd);
}


void create_dirs(boolean is_advanced)
{
    pid_t pid;

    pid = fork();
    if(pid == 0)
    {
        execlp("mkdir", "mkdir", "hdr", NULL);
        exit(1);
    }
    wait(NULL);

    pid = fork();
    if(pid == 0)
    {
        execlp("mkdir", "mkdir", "src", NULL);
        exit(1);
    }
    wait(NULL);

    pid = fork();
    if(pid == 0)
    {
        execlp("mkdir", "mkdir", "bin", NULL);
        exit(1);
    }
    wait(NULL);
    
    if(is_advanced)
    {
        pid = fork();
        if(pid == 0)
        {
            execlp("mkdir", "mkdir", "bin/debug", NULL);
            exit(1);
        }

        wait(NULL);

        pid = fork();
        if(pid == 0)
        {
            execlp("mkdir", "mkdir", "bin/release", NULL);
            exit(1);
        }
    }
    wait(NULL);
}
