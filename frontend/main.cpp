#include "../lib/tree/tree.h"
#include "../lib/tree/tree_debug.h"
#include "./include/dsl.h"
#include "./include/expr.h"

int main(void)
{
    open_log_file();

    make_ast_tree("prog.tatar");

    read_ast_tree();

    close_log_file();
    return 0;
}

