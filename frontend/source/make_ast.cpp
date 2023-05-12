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
    fclose(fp);

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
        case TYPE_DEF:
        case TYPE_FUNC:
            fprintf(stream, "%d:%s", node->type, node->name);  // add def ...
            break;
        default: fprintf(stream, "%d", node->type);
    }

    tree_print_preorder(node->left, stream);
    tree_print_preorder(node->right, stream);

    fprintf(stream, ")");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

int read_ast_tree(void)
{
    FILE* stream = fopen("ast.tatar", "r");

    if (stream == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file ast.tatar for tree print</pre>\n");
        return 1;
    }

    prog_tree_t prog = {};

    prog.ids = (id_item_t**) calloc(IDS_MAX_CNT, sizeof(id_item_t*));
    prog.tree = (tree_t*) calloc(1, sizeof(tree_t));

    tree_ctor(prog.tree);

    //=========================
    fseek(stream, 0L, SEEK_END);
    size_t buffsize = (size_t) ftell(stream);
    rewind(stream);

    prog.buffer = (char*) calloc(buffsize + 1, sizeof(char));
    fread(prog.buffer, sizeof(char), buffsize, stream);

    //printf("\nptr: %p, buffer: %s, buffsize: %lu", stream, prog.buffer, buffsize);

    link_root(prog.tree, read_tree_preorder(&prog));

    tree_dump(prog.tree, nullptr);

    tree_dtor(prog.tree);
    free(prog.ids);
    free(prog.buffer);
    fclose(stream);

    return STATUS_OK;
}

tree_node_t* read_tree_preorder(prog_tree_t* prog)
{
    if (prog->buffer[prog->pos] == '(')
    {
        //printf("%c", prog->buffer[prog->pos]);
        prog->pos++;
        if (prog->buffer[prog->pos] == '(') return read_tree_preorder(prog);

        int type = 0, cnt = 0;
        sscanf(prog->buffer + prog->pos, "%d%n", &type, &cnt);
        prog->pos += (size_t) cnt;

        printf("%d\n", type);

        switch (type)
        {
            case TYPE_NUM:
            {
                prog->pos++;
                elem_t value = NAN;
                sscanf(prog->buffer + prog->pos, "%lg%n", &value, &cnt);
                prog->pos += (size_t) cnt + 1;

                return create_node(TYPE_NUM, 0, 0, value);
            }
            case TYPE_FUNC:
            {
                printf("I am in func\n");
                prog->pos++;
                char* name = (char*) calloc(NAME_MAX_LEN, sizeof(char));

                sscanf(prog->buffer + prog->pos, "%[a-zA-Z]%n", name, &cnt);
                prog->pos += (size_t) cnt;

                tree_node_t* func = create_node(TYPE_FUNC, 0, 0);
                func->name = name;

                func->left = read_tree_preorder(prog);
                prog->pos++;

                return func;
            }
            case TYPE_PRINTF: case TYPE_SCANF:
            {
                tree_node_t* left = read_tree_preorder(prog);
                prog->pos++;

                return create_node((node_type) type, 0, 0, NAN, left);
            }
            case TYPE_VAR:
            {
                prog->pos++;
                char* name = (char*) calloc(NAME_MAX_LEN, sizeof(char));
                sscanf(prog->buffer + prog->pos, "%[a-zA-Z]%n", name, &cnt);
                prog->pos += (size_t) cnt + 1;

                tree_node_t* var = create_node(TYPE_VAR, 0, 0);
                if (var != nullptr)
                    var->name = name;

                return var;
            }
            case TYPE_DEF:
            {
                prog->pos++;
                char* name = (char*) calloc(NAME_MAX_LEN, sizeof(char));
                sscanf(prog->buffer + prog->pos, "%[a-zA-Z]%n", name, &cnt);
                prog->pos += (size_t) cnt;

                tree_node_t* var  = read_tree_preorder(prog);
                tree_node_t* body = read_tree_preorder(prog);

                tree_node_t* def = create_node(TYPE_DEF, 0, 0, NAN, var, body);
                def->name = name;

                prog->pos++;

                return def;
            }
            default:
            {
                tree_node_t* left  = read_tree_preorder(prog);
                tree_node_t* right = read_tree_preorder(prog);

                prog->pos++;

                return create_node((node_type) type, 0, 0, NAN, left, right);
            }

        }
    }

    return nullptr;
}
