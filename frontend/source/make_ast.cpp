#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/expr.h"
#include "../include/text_funcs.h"
#include "../include/dsl.h"
#include "../include/get.h"

int make_ast_tree(const char* filename)
{
    FILE* fp = fopen("ast.tatar", "w");

    if (fp == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file ast.tatar for tree print</pre>\n");
        return 1;
    }

    expr_t* expr = expr_ctor(filename);

    ASSERT(expr);
    ASSERT(expr->tree);

    tree_print_preorder(expr->tree->root, fp);

    expr_dtor(expr);

    return STATUS_OK;
}

void tree_print_preorder(tree_node_t* node, FILE* stream)
{
    if (node == nullptr) return;

    fprintf(stream, "(");

    switch (node->type)
    {
        case TYPE_NUM: fprintf(stream, "%d:%lg", node->type, node->value); break;
        case TYPE_VAR:
        case TYPE_FUNC:
            fprintf(stream, "%d:%s", node->type, node->name);
            break;
        default: fprintf(stream, "%d", node->type);
    }

    tree_print_preorder(node->left, stream);
    tree_print_preorder(node->right, stream);

    fprintf(stream, ")");
}
