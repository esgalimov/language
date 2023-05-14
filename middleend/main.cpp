#include "create_asm.h"

int main(void)
{
    if (open_log_file()) return 1;

    read_ast_tree();
    close_log_file();

    return 0;
}
