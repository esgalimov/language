#include "asm_header.h"

int main(void)
{
    FILE * fp = NULL;
    fp = fopen("../test.asm", "r");
    if (fp == NULL)
    {
        printf("Can't open file");
        return 1;
    }
    run_comp(fp);

    fclose(fp);
    return 0;
}
