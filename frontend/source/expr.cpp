#include "../../lib/tree/tree.h"
#include "../../lib/tree/tree_debug.h"
#include "../include/expr.h"
#include "../../lib/text_funcs/text_funcs.h"
#include "../include/dsl.h"
#include "../include/get.h"

expr_t* expr_ctor(const char* filename)
{
    ASSERT(filename);

    FILE * fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for read tree</pre>\n", filename);
        return NULL;
    }

    expr_t* expr = (expr_t*) calloc(1, sizeof(expr_t));

    expr->pos = 0;
    expr->line = 0;
    expr->id_cnt = 0;
    expr->ids = (id_item_t**) calloc(IDS_MAX_CNT, sizeof(id_item_t*));

    expr->program = (text_t*) calloc(1, sizeof(text_t));
    text_ctor(expr->program, fp);

    create_tokens(expr);

    tree_t* tree = (tree_t*) calloc(1, sizeof(tree_t));
    tree_ctor(tree);

    expr->tree = tree;

    tokens_dump(expr);

    link_root(expr->tree, getG(expr));

    tree_dump(expr->tree);

    return expr;
}

int expr_dtor(expr_t* expr)
{
    ASSERT(expr);

    for (size_t i = 0; i < expr->id_cnt; i++)
        free(expr->ids[i]);
    free(expr->ids);

    expr->id_cnt   = POISON;
    expr->pos      = POISON;
    expr->line     = POISON;
    expr->toks_cnt = POISON;

    tree_dtor(expr->tree);
    text_dtor(expr->program);

    return STATUS_OK;
}

int create_tokens(expr_t* expr)
{
    ASSERT(expr);

    expr->tokens = (tree_node_t**) calloc(TOKS_MAX_CNT, sizeof(tree_node_t*));

    for (expr->line = 0; expr->line < expr->program->str_cnt; expr->line++)
    {
        size_t len_str = strlen(expr->program->strings[expr->line]);
        //printf("%s\n", expr->program->strings[expr->line]);

        for (expr->pos = 0; expr->pos < len_str; expr->pos++)
        {
            //printf("line: %lu pos: %lu ch: %d - %c\n", expr->line, expr->pos, CURR_CH, CURR_CH);

            if (isblank(CURR_CH) || CURR_CH == '\0' || CURR_CH == '\n') continue;

            switch (CURR_CH)
            {
                case '+': NEW_CHAR_TOKEN(TYPE_ADD);    break;
                case '-': NEW_CHAR_TOKEN(TYPE_SUB);    break;
                case '*': NEW_CHAR_TOKEN(TYPE_MUL);    break;
                case '/': NEW_CHAR_TOKEN(TYPE_DIV);    break;
                case '(': NEW_CHAR_TOKEN(TYPE_L_BR);   break;
                case ')': NEW_CHAR_TOKEN(TYPE_R_BR);   break;
                case ';': NEW_CHAR_TOKEN(TYPE_AND);    break;
                case '=': NEW_CHAR_TOKEN(TYPE_ASSIG);  break;
                default:
                {
                    if ('0' <= CURR_CH && CURR_CH <= '9')
                    {
                        expr->tokens[expr->toks_cnt++] = NEW_NUM(read_number(expr));
                        break;
                    }
                    char* name = read_name(expr);

                    if      (!strcasecmp(name, "esh"))       NEW_WORD_TOKEN(TYPE_DEF);
                    else if (!strcasecmp(name, "bir"))       NEW_WORD_TOKEN(TYPE_RET);
                    else if (!strcasecmp(name, "tugyaryak")) NEW_WORD_TOKEN(TYPE_WHILE);
                    else if (!strcasecmp(name, "egyar"))     NEW_WORD_TOKEN(TYPE_IF);
                    else if (!strcasecmp(name, "bylmasa"))   NEW_WORD_TOKEN(TYPE_ELSE);
                    else if (!strcasecmp(name, "yozyrga"))   NEW_WORD_TOKEN(TYPE_PRINTF);
                    else if (!strcasecmp(name, "ukyrga"))    NEW_WORD_TOKEN(TYPE_SCANF);
                    else if (!strcasecmp(name, "bashlau"))   NEW_WORD_TOKEN(TYPE_BEGIN);
                    else if (!strcasecmp(name, "tuktau"))    NEW_WORD_TOKEN(TYPE_END);

                    else
                    {
                        int id = find_id(expr, name);

                        if (id != NO_ID)
                        {
                            expr->tokens[expr->toks_cnt] = create_node(TYPE_ID, expr->line, expr->pos, elem_t (id));
                            expr->tokens[expr->toks_cnt]->name = name;
                        }
                        else if (expr->id_cnt <= IDS_MAX_CNT - 1)
                        {
                            expr->ids[expr->id_cnt] = (id_item_t*) calloc(1, sizeof(id_item_t));
                            expr->ids[expr->id_cnt]->value = NAN;
                            expr->ids[expr->id_cnt]->name = name;
                            expr->ids[expr->id_cnt]->type = TYPE_ID;

                            expr->tokens[expr->toks_cnt] = create_node(TYPE_ID, expr->line, expr->pos, (elem_t) expr->id_cnt++);
                            expr->tokens[expr->toks_cnt]->name = name;
                        }
                        else
                        {
                            fprintf(log_file, "<pre>Variables number is max - %d</pre>\n", IDS_MAX_CNT);
                        }
                        expr->toks_cnt++;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}

char* read_name(expr_t* expr)
{
    ASSERT(expr);

    char * name = (char *) calloc(NAME_MAX_LEN, sizeof(char));
    int i = 0;

    while ((('a' <= CURR_CH && CURR_CH <= 'z') || ('A' <= CURR_CH && CURR_CH <= 'Z')) && i < NAME_MAX_LEN)
        name[i++] = expr->program->strings[expr->line][expr->pos++];

    expr->pos--;
    return name;
}

double read_number(expr_t* expr)
{
    double val = 0;
    int d_after_dot = -1;

    while (('0' <= CURR_CH && CURR_CH <= '9') || CURR_CH == '.')
    {
        if (d_after_dot >= 0)
            d_after_dot++;

        if (CURR_CH == '.')
        {
            if (d_after_dot == -1)
                d_after_dot++;
            else
            {
                fprintf(log_file, "<pre>Func: read_number; ERROR: second \".\" in number, line = %lu, pos = %lu\n</pre>", expr->line, expr->pos);
                return NAN;
            }
        }
        else
            val = val * 10 + CURR_CH - '0';

        expr->pos++;
    }
    double ret_val = val / pow(10, (d_after_dot == -1) ? 0 : d_after_dot);
    expr->pos--;

    return ret_val;
}

int find_id(expr_t* expr, const char* name)
{
    ASSERT(expr);

    for (int i = 0; i < (int) expr->id_cnt; i++)
    {
        if (!strcmp(expr->ids[i]->name, name))
            return i;
    }
    return NO_ID;
}

int tokens_dump(expr_t* expr)
{
    ASSERT(expr);

    dump_cnt++;
    char graphviz_cmd[200] = "dot ./tmp/graphviz.dot -Tpng -o ./logs/images/tree_dump";
    snprintf(graphviz_cmd + strlen(graphviz_cmd), 30, "%d.png", dump_cnt);


    open_graphviz_file();
    graphviz_init(expr->tree);

    printf("%lu", expr->toks_cnt);

    for (size_t i = 0; i < expr->toks_cnt; i++)
        add_nodes(expr->tokens[i]);

    close_graphviz_file();

    system(graphviz_cmd);

    fprintf(log_file, "<img src=\"./images/tree_dump%d.png\">\n", dump_cnt);

    return STATUS_OK;
}

void new_word_token(expr_t* expr, node_type type, char* name)
{
    ASSERT(expr);

    expr->tokens[expr->toks_cnt] = create_node(type, expr->line, expr->pos);
    expr->tokens[expr->toks_cnt]->name = name;
    expr->toks_cnt++;
}

void new_char_token(expr_t* expr, node_type type, char ch)
{
    ASSERT(expr);

    expr->tokens[expr->toks_cnt] = create_node(type, expr->line, expr->pos);
    expr->tokens[expr->toks_cnt]->name = (char*) calloc(2, sizeof(char));
    expr->tokens[expr->toks_cnt]->name[0] = ch;
    expr->toks_cnt++;
}
