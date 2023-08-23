#include "create_asm.h"
#include "simplify.h"


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

    fseek(stream, 0L, SEEK_END);
    size_t buffsize = (size_t) ftell(stream);
    rewind(stream);

    prog.buffer = (char*) calloc(buffsize + 1, sizeof(char));
    fread(prog.buffer, sizeof(char), buffsize, stream);

    link_root(prog.tree, read_tree_preorder(&prog));

    dump_cnt += 100;
    tree_dump(prog.tree);

    tree_simplify(&prog, &prog.tree->root);

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
                sscanf(prog->buffer + prog->pos, "%[_0-9A-Za-z]%n", name, &cnt);
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
                sscanf(prog->buffer + prog->pos, "%[_0-9A-Za-z]%n", name, &cnt);
                prog->pos += (size_t) cnt;

                tree_node_t* child1  = read_tree_preorder(prog);
                tree_node_t* child2 = read_tree_preorder(prog);

                tree_node_t* def = create_node(TYPE_DEF, 0, 0);
                def->name = name;

                if (child2 == nullptr)
                {
                    def->right = child1;
                }
                else
                {
                    def->left = child1;
                    def->right = child2;
                }
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

    tree_print_asm(prog->tree->root, prog, asm_file);

    fprintf(asm_file, "    hlt\n");
    fclose(asm_file);

    return 0;
}

void tree_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream)
{
    static int if_cnt = 0,
            while_cnt = 0,
             else_cnt = 0,
            logic_cnt = 0;

    if (node == nullptr) return;

    switch (node->type)
    {
        case TYPE_AND:
            tree_print_asm_l();
            tree_print_asm_r();
            return;

        case TYPE_NUM:
            fprintf(stream, "    push %lg\n", node->value);
            return;

        case TYPE_VAR:
            fprintf(stream, "    push [%d+dx]\n", (int) node->value);
            return;

        case TYPE_ASSIG:
            tree_print_asm_r();
            fprintf(stream, "    pop [%d+dx]\n", (int) node->left->value);
            return;

        case TYPE_ADD: case TYPE_SUB: case TYPE_MUL: case TYPE_DIV:
            tree_print_asm_l();
            tree_print_asm_r();

            switch (node->type)
            {
                case TYPE_ADD: fprintf(stream, "    add\n"); return;
                case TYPE_SUB: fprintf(stream, "    sub\n"); return;
                case TYPE_MUL: fprintf(stream, "    mul\n"); return;
                case TYPE_DIV: fprintf(stream, "    div\n"); return;
            }

        case TYPE_EQ: case TYPE_NEQ: case TYPE_GE:
        case TYPE_G:  case TYPE_LE:  case TYPE_L:
            tree_print_asm_l();
            tree_print_asm_r();

            switch (node->type)
            {
                case TYPE_EQ:
                    fprintf(stream, "    jne :logic_jmp_%d\n", logic_cnt++);  break;
                case TYPE_NEQ:
                    fprintf(stream, "    je  :logic_jmp_%d\n", logic_cnt++);  break;
                case TYPE_GE:
                    fprintf(stream, "    jb  :logic_jmp_%d\n", logic_cnt++);  break;
                case TYPE_G:
                    fprintf(stream, "    jbe  :logic_jmp_%d\n", logic_cnt++); break;
                case TYPE_LE:
                    fprintf(stream, "    ja  :logic_jmp_%d\n", logic_cnt++);  break;
                case TYPE_L:
                    fprintf(stream, "    jae :logic_jmp_%d\n", logic_cnt++);  break;
            }

            fprintf(stream, "    push 1\n");
            fprintf(stream, "    jmp :logic_jmp_%d\n", logic_cnt);
            fprintf(stream, "    :logic_jmp_%d\n    push 0\n", logic_cnt - 1);
            fprintf(stream, "    :logic_jmp_%d\n", logic_cnt++);

            return;

        case TYPE_PRINTF:
            tree_print_asm_l();
            fprintf(stream, "    out\n");
            return;

        case TYPE_SCANF:
            fprintf(stream, "    in\n");
            fprintf(stream, "    pop [%d+dx]\n", (int) node->left->value);
            return;

        case TYPE_FUNC:
            push_params_in_func(node->left, prog, stream);

            fprintf(stream, "\n    push dx\n    push %lu\n", prog->var_cnt);
            fprintf(stream, "    add\n    pop dx\n\n");

            fprintf(stream, "    call :%s\n", node->name);
            fprintf(stream, "    push ax\n");

            fprintf(stream, "\n    push dx\n    push %lu\n", prog->var_cnt);
            fprintf(stream, "    sub\n    pop dx\n\n");
            return;

        case TYPE_DEF:
            fprintf(stream, "    jmp :jmp_over_%s\n", node->name);
            fprintf(stream, "    :%s\n", node->name);

            pop_params_in_def(node->left, stream);
            tree_print_asm_r();

            fprintf(stream, "    ret\n");
            fprintf(stream, "    :jmp_over_%s\n", node->name);
            return;

        case TYPE_IF:
            tree_print_asm_l();
            fprintf(stream, "    push 0\n");
            fprintf(stream, "    je :if_%d\n", if_cnt);

            if (node->right->type == TYPE_AND && node->right->right->type == TYPE_ELSE)
            {
                tree_print_asm(node->right->left, prog, stream);
                fprintf(stream, "    :if_%d\n", if_cnt++);
                tree_print_asm_l();
                tree_print_asm(node->right->right, prog, stream);
            }
            else
            {
                tree_print_asm_r();
                fprintf(stream, "    :if_%d\n", if_cnt++);
            }
            return;

        case TYPE_ELSE:
            fprintf(stream, "    push 0\n");
            fprintf(stream, "    jne :else_%d\n", else_cnt);
            tree_print_asm_l();
            fprintf(stream, "    :else_%d\n", else_cnt);
            else_cnt++;
            return;

        case TYPE_WHILE:
            fprintf(stream, "    :while_%d\n", while_cnt);
            tree_print_asm_l();

            fprintf(stream, "    push 0\n");
            fprintf(stream, "    je :while_%d\n", while_cnt + 1);

            tree_print_asm_r();
            fprintf(stream, "    jmp :while_%d\n", while_cnt);
            fprintf(stream, "    :while_%d\n", while_cnt + 1);

            while_cnt += 2;
            return;

        case TYPE_RET:
            tree_print_asm_l();
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
        fprintf(stream, "    pop [%d+dx]\n", (int) node->value);

    pop_params_in_def(node->left, stream);
    pop_params_in_def(node->right, stream);
}

void push_params_in_func(tree_node_t* node, prog_tree_t* prog, FILE* stream)
{
    if (node == nullptr) return;

    switch (node->type)
    {
        case TYPE_AND:
            push_params_in_func(node->right, prog, stream);
            push_params_in_func(node->left,  prog, stream);
            break;

        case TYPE_NUM:
            fprintf(stream, "    push %lg\n", node->value);
            break;

        case TYPE_VAR:
            fprintf(stream, "    push [%d+dx]\n", (int) node->value);
            break;

        default:
            if ((TYPE_ADD <= node->type && node->type <= TYPE_DIV) ||
                (TYPE_EQ  <= node->type && node->type <= TYPE_L))
                tree_print_asm(node, prog, stream);

            else return;
    }
}

int func_params_cnt(tree_node_t* node)
{
    if (node == nullptr) return 0;

    if (node->type == TYPE_AND)
        return func_params_cnt(node->left) + func_params_cnt(node->right);

    if (node->type == TYPE_VAR) return 1;

    else return 0;
}
