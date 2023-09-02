#include "create_asm.h"
#include "simplify.h"
#include "../lib/text_funcs/text_funcs.h"


int read_ast_tree(void)
{
    FILE* stream = fopen("./system_files/prog.ast", "r");

    if (stream == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file prog.ast for tree print</pre>\n");
        return 1;
    }

    prog_tree_t prog = {};

    prog.vars  = (var_t*) calloc(IDS_MAX_CNT, sizeof(var_t));
    prog.var_cnt = 0;
    prog.part = GLOBAL;

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
                prog->part = LOCAL;
                prog->pos++;

                char* name = (char*) calloc(NAME_MAX_LEN, sizeof(char));
                sscanf(prog->buffer + prog->pos, "%[_0-9A-Za-z]%n", name, &cnt);
                prog->pos += (size_t) cnt;

                tree_node_t* child1 = read_tree_preorder(prog);
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

                prog->part = GLOBAL;

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
        if (!strcmp(prog->vars[i].name, name) && (prog->vars[i].global == GLOBAL ||
                   (prog->vars[i].global == LOCAL && prog->part == LOCAL)))
            return i;
    }
    prog->vars[prog->var_cnt].name   = name;
    prog->vars[prog->var_cnt].global = prog->part;

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

    tree_print_asm(prog->tree->root, prog, asm_file);

    fclose(asm_file);

    asm_file = fopen("./system_files/prog.asm", "r");
    if (asm_file == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file prog.asm to make asm file</pre>\n");
        return 1;
    }

    asm_change(asm_file, prog);

    return 0;
}

void tree_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream)
{
    static int if_cnt = 0,
            while_cnt = 0,
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
            fprintf(stream, "    push");
            var_print_asm(node, prog, stream);

            return;

        case TYPE_ASSIG:
            tree_print_asm_r();
            fprintf(stream, "    pop");
            var_print_asm(node->left, prog, stream);
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
            fprintf(stream, "    pop");
            var_print_asm(node->left, prog, stream);

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
            fprintf(stream, "*\n");

            fprintf(stream, ":%s\n", node->name);

            pop_params_in_def(node->left, prog, stream);
            tree_print_asm_r();

            fprintf(stream, "    ret\n\n");

            fprintf(stream, "*\n");
            return;

        case TYPE_IF:
        {
            int if_cnt_saved = ++if_cnt;

            tree_print_asm_l();
            fprintf(stream, "    push 0\n");
            fprintf(stream, "    je :false_%d\n", if_cnt_saved);

            if (node->right->type == TYPE_AND && node->right->right->type == TYPE_ELSE)
            {
                tree_print_asm(node->right->left, prog, stream);
                fprintf(stream, "    jmp :done_%d\n", if_cnt_saved);
                fprintf(stream, "    :false_%d\n", if_cnt_saved);
                tree_print_asm(node->right->right->left, prog, stream);
                fprintf(stream, "    :done_%d\n", if_cnt_saved);
            }
            else
            {
                tree_print_asm_r();
                fprintf(stream, "    :false_%d\n", if_cnt_saved);
            }
            return;
        }

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

        case TYPE_ALL:
            fprintf(stream, "    allnum\n");
            return;

        case TYPE_NO:
            fprintf(stream, "    noroots\n");
            return;

        default: return;
    }
}

void pop_params_in_def(tree_node_t* node, prog_tree_t* prog, FILE* stream)
{
    if (node == nullptr) return;

    if (node->type == TYPE_VAR)
    {
        fprintf(stream, "    pop");
        var_print_asm(node, prog, stream);
    }

    pop_params_in_def(node->left,  prog, stream);
    pop_params_in_def(node->right, prog, stream);
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
            fprintf(stream, "    push");
            var_print_asm(node, prog, stream);
            break;

        default:
            if ((TYPE_ADD <= node->type && node->type <= TYPE_DIV) ||
                (TYPE_EQ  <= node->type && node->type <= TYPE_L))
                tree_print_asm(node, prog, stream);

            else return;
    }
}

void var_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream)
{
    int i_var = (int) node->value;

    if (prog->vars[i_var].global == LOCAL)
        fprintf(stream, " [%d+dx]\n", i_var);
    else
        fprintf(stream, " [%d]\n", i_var);
}

void asm_change(FILE* stream, prog_tree_t* prog)
{
    text_t asm_text = {};

    text_ctor(&asm_text, stream);

    stream = fopen("./system_files/prog.asm", "w");
    if (stream == nullptr)
    {
        fprintf(log_file, "<pre>Can't open file prog.asm to make asm file</pre>\n");
        return;
    }

    fprintf(stream, ";Created by Emil Galimov\n;2023\n;Tatar programming language version 1.0\n\n;VARS TABLE:\n");
    for (size_t i = 0; i < prog->var_cnt; i++)
    {
        fprintf(stream, "    ;var %s - [%lu]", prog->vars[i].name, i);
        if (prog->vars[i].global == GLOBAL)
            fprintf(stream, " gl\n");
        else
            fprintf(stream, " loc\n");
    }
    fprintf(stream, "\n\n\n");

    char** main_part = (char**) calloc(asm_text.str_cnt, sizeof(char*));
    int i_main = 0;

    char in_def = -1;

    fprintf(stream, "    jmp :main\n");

    for (size_t i = 0; i < asm_text.str_cnt; i++)
    {
        if (asm_text.strings[i][0] == '*') { in_def *= -1; continue; }

        if (in_def == -1)
            main_part[i_main++] = asm_text.strings[i];

        else
            fprintf(stream, "%s\n", asm_text.strings[i]);

    }

    fprintf(stream, ":main\n");
    for (int i = 0; i < i_main; i++)
        fprintf(stream, "%s\n", main_part[i]);

    fprintf(stream, "    hlt\n");

    fclose(stream);

    text_dtor(&asm_text);
}
