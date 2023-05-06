#include "./include/tree.h"
#include "./include/tree_debug.h"
#include "./include/dsl.h"

int main(void)
{
    open_log_file();

    expr_ctor("prog.tatar");

    close_log_file();
    return 0;
}

