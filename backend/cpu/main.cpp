#include "cpu.h"

FILE * log_file = NULL;

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf("file name not given\n");
        return 1;
    }

    log_file = fopen("./backend/cpu/logs/log.txt", "w");
    if (log_file == NULL)
    {
        printf("Can't open log file (cpu)");
        return 1;
    }

    FILE * fp_bin = fopen(argv[1], "rb");

    if (fp_bin == nullptr)
    {
        printf("Can not open file %s\n", argv[1]);
        return 1;
    }

    run_cpu(fp_bin);

    fclose(fp_bin);
    fclose(log_file);
    return 0;
}
