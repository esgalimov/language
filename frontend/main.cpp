#include "./include/tree.h"
#include "./include/tree_debug.h"
#include "./include/dsl.h"

int main(void)
{
    open_log_file();

    make_ast_tree("prog.tatar");

    read_ast_tree();

    close_log_file();
    return 0;
}

