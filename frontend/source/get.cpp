#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/dsl.h"
#include "../include/get.h"

tree_node_t * getG(expr_t * expr)
{
    tree_node_t * val = getE(expr);

    if (expr->buffer[expr->pos] != '$' && val != NULL)
    {
        fprintf(log_file, "<pre>Func: getG; ERROR: must be \"$\" in the end; pos = %d, ch = %c\n</pre>",
                           expr->pos, expr->buffer[expr->pos]);
        return NULL;
    }
    return val;
}

tree_node_t * getE(expr_t * expr)
{
    tree_node_t * val = getT(expr);
    while (expr->buffer[expr->pos] == '+' || expr->buffer[expr->pos] == '-')
    {
        char op = expr->buffer[expr->pos];
        expr->pos++;
        tree_node_t * val2 = getT(expr);
        if (op == '+') val = ADD(val, val2);
        else           val = SUB(val, val2);
    }
    return val;
}

tree_node_t * getT(expr_t * expr)
{
    tree_node_t * val = getD(expr);
    while (expr->buffer[expr->pos] == '*'|| expr->buffer[expr->pos] == '/')
    {
        char op = expr->buffer[expr->pos];
        expr->pos++;
        tree_node_t * val2 = getD(expr);
        if (op == '*') val = MUL(val, val2);
        else
        {
            if (val2->type == TYPE_NUM && is_equal(val2->value, 0))
            {
                fprintf(log_file, "<pre>Func: getT; ERROR: division by zero; pos = %d\n</pre>", expr->pos);
                return NULL;
            }
            val = DIV(val, val2);
        }
    }
    return val;
}

tree_node_t * getD(expr_t * expr)
{
    tree_node_t * val = getP(expr);

    while (expr->buffer[expr->pos] == '^')
    {
        expr->pos++;
        tree_node_t * val2 = getP(expr);
        val = POW(val, val2);
    }
    return val;
}

tree_node_t * getP(expr_t * expr)
{
    if (expr->buffer[expr->pos] == '(')
    {
        expr->pos++;
        tree_node_t * val = getE(expr);
        if (expr->buffer[expr->pos] != ')')
        {
            fprintf(log_file, "<pre>Func: getP; ERROR: no \")\", pos = %d, ch = %c\n</pre>",
                               expr->pos, expr->buffer[expr->pos]);
            return NULL;
        }
        expr->pos++;
        return val;
    }
    else if (('0' <= expr->buffer[expr->pos] && expr->buffer[expr->pos] <= '9') || expr->buffer[expr->pos] == '-')
        return getN(expr);
    else
        return getW(expr);
}

tree_node_t * getW(expr_t * expr)
{
    char * name = read_name(expr);

    if (expr->buffer[expr->pos] == '(')
    {
        expr->pos++;
        tree_node_t * func_arg = getE(expr);

        if (expr->buffer[expr->pos] != ')')
        {
            fprintf(log_file, "<pre>Func: GetW; ERROR: no \")\", pos = %d, ch = %c</pre>\n",
                    expr->pos, expr->buffer[expr->pos]);
            return NULL;
        }
        expr->pos++;
        if (!strcasecmp(name, "sin")) return SIN(func_arg);
        if (!strcasecmp(name, "cos")) return COS(func_arg);
        if (!strcasecmp(name, "ln"))  return LN(func_arg);
        if (!strcasecmp(name, "exp")) return EXP(func_arg);
    }

    int var_id = find_var(expr, name);

    if (var_id != NO_VAR) return VAR(var_id);

    else if (expr->var_cnt <= VARS_MAX_CNT - 1)
    {
        expr->vars[expr->var_cnt] = (var_t*) calloc(1, sizeof(var_t));
        expr->vars[expr->var_cnt]->value = NAN;
        expr->vars[expr->var_cnt]->name = name;

        return VAR(expr->var_cnt++);
    }
    else
    {
        fprintf(log_file, "<pre>Variables number is max - %d</pre>\n", VARS_MAX_CNT);
        return NULL;
    }
}

tree_node_t * getN(expr_t * expr)
{
    double val = 0;
    int saved_p = expr->pos;
    int is_neg = 0, d_after_dot = -1;

    if (expr->buffer[expr->pos] == '-')
    {
        is_neg = 1;
        expr->pos++;
    }

    while (('0' <= expr->buffer[expr->pos] && expr->buffer[expr->pos] <= '9') || expr->buffer[expr->pos] == '.')
    {
        if (d_after_dot >= 0)
            d_after_dot++;

        if (expr->buffer[expr->pos] == '.')
        {
            if (d_after_dot == -1)
                d_after_dot++;
            else
            {
                fprintf(log_file, "<pre>Func: getN; ERROR: second \".\" in number, pos = %d\n</pre>", expr->pos);
                return NULL;
            }
        }
        else
            val = val * 10 + expr->buffer[expr->pos] - '0';

        expr->pos++;
    }
    if (expr->pos == saved_p)
    {
        fprintf(log_file, "<pre>Func: getN; ERROR: digit didn't find; pos = %d, ch = %c\n</pre>",
                           expr->pos, expr->buffer[expr->pos]);
        return NULL;
    }
    double ret_val = (is_neg ? -1 : 1) * val / pow(10, (d_after_dot == -1) ? 0 : d_after_dot);
    return NUM(ret_val);
}

char * read_name(expr_t * expr)
{
    char * name = (char *) calloc(NAME_MAX_LEN, sizeof(char));
    int i = 0;

    while (('a' <= expr->buffer[expr->pos] && expr->buffer[expr->pos] <= 'z') ||
           ('A' <= expr->buffer[expr->pos] && expr->buffer[expr->pos] <= 'Z'))
        name[i++] = expr->buffer[expr->pos++];

    return name;
}

int find_var(expr_t* expr, const char* name)
{
    for (int i = 0; i < expr->var_cnt; i++)
    {
        if (!strcmp(expr->vars[i]->name, name))
            return i;
    }
    return NO_VAR;
}

int is_equal(double num1, double num2)
{
    ASSERT(isfinite(num1));
    ASSERT(isfinite(num2));

    return fabs(num1 - num2) < EPS;
}
