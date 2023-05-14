#include "cpu.h"

FILE * log_file = NULL;

int main(void)
{
    log_file = fopen("../cpu/logs/log.txt", "w");
    if (log_file == NULL)
    {
        printf("Can't open log file!");
        return 1;
    }

    FILE * fp_bin = fopen("../code/test.bin", "rb");
    if (fp_bin == NULL)
    {
        printf("Can not open file\n");
        return 1;
    }

    run_cpu(fp_bin);

    fclose(fp_bin);
    fclose(log_file);
    return 0;
}
