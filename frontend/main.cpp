#include "../lib/tree/tree.h"
#include "../lib/tree/tree_debug.h"
#include "./include/dsl.h"
#include "./include/expr.h"

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf("file name not given\n");
        return 1;
    }
    if (open_log_file()) return 1;

    make_ast_tree(argv[1]);

    close_log_file();
    return 0;
}

