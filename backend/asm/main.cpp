#include "asm.h"
#include "../../lib/text_funcs/text_funcs.h"

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf("file name not given\n");
        return 1;
    }
    FILE * fp = fopen(argv[1], "r");

    if (fp == nullptr)
    {
        printf("Can't open file %s\n", argv[1]);
        return 1;
    }
    run_comp(fp);

    fclose(fp);
    return 0;
}
