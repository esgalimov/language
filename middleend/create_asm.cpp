#include "create_asm.h"

int read_ast_tree(void)
{
    FILE* stream = fopen("./system_files/ast.tatar", "r");

    if (stream == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file ast.tatar for tree print</pre>\n");
        return 1;
    }

    prog_tree_t prog = {};

    prog.vars  = (char**) calloc(IDS_MAX_CNT, sizeof(char*));
    prog.var_cnt = 0;

    prog.tree = (tree_t*) calloc(1, sizeof(tree_t));

    tree_ctor(prog.tree);

    //=========================
    fseek(stream, 0L, SEEK_END);
    size_t buffsize = (size_t) ftell(stream);
    rewind(stream);

    prog.buffer = (char*) calloc(buffsize + 1, sizeof(char));
    fread(prog.buffer, sizeof(char), buffsize, stream);

    link_root(prog.tree, read_tree_preorder(&prog));

    dump_cnt += 100;
    tree_dump(prog.tree);

    translate_asm(&prog);

    tree_dtor(prog.tree);
    free(prog.vars);
    free(prog.buffer);
    fclose(stream);

    return STATUS_OK;
}

tree_node_t* read_tree_preorder(prog_tree_t* prog)
{
    if (prog->buffer[prog->pos] == '(')
    {
        prog->pos++;
        if (prog->buffer[prog->pos] == '(') return read_tree_preorder(prog);

        int type = 0, cnt = 0;
        sscanf(prog->buffer + prog->pos, "%d%n", &type, &cnt);
        prog->pos += (size_t) cnt;

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

                if (var == nullptr) return nullptr;

                var->name = name;
                var->value = find_var(prog, name);

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

size_t find_var(prog_tree_t* prog, char* name)
{
    for (size_t i = 0; i < prog->var_cnt; i++)
    {
        if (!strcmp(prog->vars[i], name)) return i;
    }
    prog->vars[prog->var_cnt] = name;

    return prog->var_cnt++;
}

int translate_asm(prog_tree_t* prog)
{
    ASSERT(prog);

    FILE* asm_file = fopen("./system_files/prog.asm", "w");
    if (asm_file == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file prog.asm to make asm file</pre>\n");
        return 1;
    }
    fprintf(asm_file, ";Created by Emil Galimov\n;2023\n;Tatar programming language version 1.0\n\n;VARS TABLE:\n");
    for (size_t i = 0; i < prog->var_cnt; i++)
    {
        fprintf(asm_file, "    ;var %s - [%lu]\n", prog->vars[i], i);
    }
    fprintf(asm_file, "\n\n\n");

    tree_print_asm(prog->tree->root, asm_file);

    fprintf(asm_file, "    hlt\n");
    fclose(asm_file);

    return 0;
}

void tree_print_asm(tree_node_t* node, FILE* stream)
{
    static int    if_cnt = 0;
    static int while_cnt = 0;
    static int  else_cnt = 0;

    if (node == nullptr) return;

    switch (node->type)
    {
        case TYPE_AND:
            tree_print_asm(node->left, stream);
            tree_print_asm(node->right, stream);
            return;

        case TYPE_NUM:
            fprintf(stream, "    push %lg\n", node->value);
            return;

        case TYPE_VAR:
            fprintf(stream, "    push [%d]\n", (int) node->value);
            return;

        case TYPE_ASSIG:
            tree_print_asm(node->right, stream);
            fprintf(stream, "    pop [%d]\n", (int) node->left->value);
            return;

        case TYPE_ADD: case TYPE_SUB: case TYPE_MUL: case TYPE_DIV:
            tree_print_asm(node->left, stream);
            tree_print_asm(node->right, stream);

            switch (node->type)
            {
                case TYPE_ADD: fprintf(stream, "    add\n"); return;
                case TYPE_SUB: fprintf(stream, "    sub\n"); return;
                case TYPE_MUL: fprintf(stream, "    mul\n"); return;
                case TYPE_DIV: fprintf(stream, "    div\n"); return;
            }

        case TYPE_PRINTF:
            tree_print_asm(node->left, stream);
            fprintf(stream, "    out\n");
            return;

        case TYPE_SCANF:
            fprintf(stream, "    in\n");
            fprintf(stream, "    pop [%d]\n", (int) node->left->value);
            return;

        case TYPE_FUNC:
            tree_print_asm(node->left, stream);
            fprintf(stream, "    call :%s\n", node->name);
            fprintf(stream, "    push ax\n");
            return;

        case TYPE_DEF:
            fprintf(stream, "    jmp :jmp_over_%s\n", node->name);
            fprintf(stream, "    :%s\n", node->name);
            pop_params_in_def(node->left, stream);
            tree_print_asm(node->right, stream);
            fprintf(stream, "    ret\n");
            fprintf(stream, "    :jmp_over_%s\n", node->name);
            return;

        case TYPE_IF:
            tree_print_asm(node->left, stream);
            fprintf(stream, "    push 0\n");
            fprintf(stream, "    je :if_%d\n", if_cnt);

            if (node->right->type == TYPE_AND && node->right->right->type == TYPE_ELSE)
            {
                tree_print_asm(node->right->left, stream);
                fprintf(stream, "    :if_%d\n", if_cnt);
                tree_print_asm(node->left, stream);
                tree_print_asm(node->right->right, stream);
            }
            else
            {
                tree_print_asm(node->right, stream);
                fprintf(stream, "    :if_%d\n", if_cnt);
            }

            if_cnt++;
            return;

        case TYPE_ELSE:
            fprintf(stream, "    push 0\n");
            fprintf(stream, "    jne :else_%d\n", else_cnt);
            tree_print_asm(node->left, stream);
            fprintf(stream, "    :else_%d\n", else_cnt);
            else_cnt++;
            return;

        case TYPE_WHILE:
            fprintf(stream, "    :while_%d\n", while_cnt);
            tree_print_asm(node->left, stream);

            fprintf(stream, "    push 0\n");
            fprintf(stream, "    je :while_%d\n", while_cnt + 1);

            tree_print_asm(node->right, stream);
            fprintf(stream, "    jmp :while_%d\n", while_cnt);
            fprintf(stream, "    :while_%d\n", while_cnt + 1);

            while_cnt += 2;
            return;

        case TYPE_RET:
            tree_print_asm(node->left, stream);
            fprintf(stream, "    pop ax\n");
            fprintf(stream, "    ret\n");
            return;

        default: return;
    }
}

void pop_params_in_def(tree_node_t* node, FILE* stream)
{
    if (node == nullptr) return;

    if (node->type == TYPE_VAR)
        fprintf(stream, "    pop [%d]\n", (int) node->value);

    pop_params_in_def(node->left, stream);
    pop_params_in_def(node->right, stream);
}
