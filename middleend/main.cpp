#include "create_asm.h"
#include "simplify.h"


int main(void)
{
    if (open_log_file_append()) return 1;

    read_ast_tree();
    close_log_file();

    return 0;
}
