#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf("file name not given\n");
        return 1;
    }

    system("rm ./system_files/prog.asm");
    system("rm ./system_files/prog.bin");
    system("rm ./system_files/prog.ast");

    char start_cmd[100] = "./objects/front";
    snprintf(start_cmd + strlen(start_cmd), 30, " %s", argv[1]);

    system(start_cmd);
    system("./objects/middle");
    system("./objects/asm ./system_files/prog.asm");
    system("./objects/cpu ./system_files/prog.bin");

    return 0;
}
