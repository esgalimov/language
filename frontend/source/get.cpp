#include "../include/tree.h"
#include "../include/tree_debug.h"
#include "../include/dsl.h"
#include "../include/get.h"

tree_node_t* getG(expr_t * expr)
{
    expr->pos = 0;

    if (expr->tokens[expr->pos]->type != TYPE_BEGIN)
    {
        SYNTAX_ERROR("prog must start with \"bashlau\""); return nullptr;
    }

    expr->pos++;
    tree_node_t* op = getComp(expr);

    if (expr->tokens[expr->pos]->type != TYPE_END)
    {
        SYNTAX_ERROR("prog must end with \"tuktau\""); return nullptr;
    }

    return op;
}

tree_node_t* getE(expr_t* expr)
{
    tree_node_t* val1 = getT(expr);
    while (expr->tokens[expr->pos]->type == TYPE_ADD || expr->tokens[expr->pos]->type == TYPE_SUB)
    {
        tree_node_t* oper = expr->tokens[expr->pos++];
        tree_node_t * val2 = getT(expr);

        oper->left = val1;
        val1->parent = oper;

        oper->right = val2;
        val2->parent = oper;

        val1 = oper;
    }
    return val1;
}

tree_node_t* getT(expr_t* expr)
{
    tree_node_t* val1 = getP(expr);
    while (expr->tokens[expr->pos]->type == TYPE_MUL || expr->tokens[expr->pos]->type == TYPE_DIV)
    {
        tree_node_t* oper = expr->tokens[expr->pos++];
        tree_node_t* val2 = getP(expr);

        if (oper->type == TYPE_DIV && val2->type == TYPE_NUM && is_equal(val2->value, 0))
        {
            SYNTAX_ERROR("division by zero"); return nullptr;
        }
        else
        {
            oper->left = val1;
            val1->parent = oper;

            oper->right = val2;
            val2->parent = oper;

            val1 = oper;
        }
    }
    return val1;
}

tree_node_t* getP(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type == TYPE_L_BR)
    {
        expr->pos++;
        tree_node_t* val = getE(expr);
        if (expr->tokens[expr->pos]->type != TYPE_R_BR)
        {
            SYNTAX_ERROR("expected \")\""); return nullptr;
        }
        expr->pos++;
        return val;
    }
    else if (expr->tokens[expr->pos]->type == TYPE_NUM || expr->tokens[expr->pos]->type == TYPE_SUB)
        return getN(expr);
    else
        return getId(expr);
}

tree_node_t* getId(expr_t* expr)
{
    return expr->tokens[expr->pos++];
}

tree_node_t* getN(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type == TYPE_SUB)
        expr->tokens[expr->pos++]->value *= -1;

    return expr->tokens[expr->pos++];
}

tree_node_t* getA(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type != TYPE_VAR)
    {
        SYNTAX_ERROR("expected TYPE_VAR"); return nullptr;
    }
    tree_node_t* var = expr->tokens[expr->pos++];

    if (expr->tokens[expr->pos]->type != TYPE_ASSIG)
    {
        SYNTAX_ERROR("expected TYPE_ASSIG"); return nullptr;
    }

    tree_node_t* assig = expr->tokens[expr->pos++];
    tree_node_t* value = getE(expr);

    if (value == nullptr) return nullptr;

    assig->left = var;
    assig->right = value;

    var->parent = assig;
    value->parent = assig;

    return assig;
}

tree_node_t* getIf(expr_t* expr)
{
    tree_node_t* cond = expr->tokens[expr->pos++];

    if (expr->tokens[expr->pos]->type != TYPE_L_BR)
    {
        SYNTAX_ERROR("expected \"(\""); return nullptr;
    }

    expr->pos++;
    tree_node_t* value = getE(expr);

    if (value == nullptr) return nullptr;

    if (expr->tokens[expr->pos]->type != TYPE_R_BR)
    {
        SYNTAX_ERROR("expected \")\""); return nullptr;
    }
    expr->pos++;

    if (expr->tokens[expr->pos]->type != TYPE_BEGIN)
    {
        SYNTAX_ERROR("must be \"bashlau\" in start of if body"); return nullptr;
    }

    expr->pos++;
    tree_node_t* comp = getComp(expr);

    if (comp == nullptr) return nullptr;

    if (expr->tokens[expr->pos]->type != TYPE_END)
    {
        SYNTAX_ERROR("must be \"tuktau\" in end of if body"); return nullptr;
    }
    expr->pos++;

    cond->left = value;
    value->parent = cond;

    cond->right = comp;
    comp->parent = cond;

    return cond;
}

tree_node_t* getOp(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type == TYPE_IF)
        return getIf(expr);

    return getA(expr);
}

// tree_node_t* getOp(expr_t* expr)
// {
//     tree_node_t* ret = nullptr;
//
//     if (expr->tokens[expr->pos]->type == TYPE_IF)
//         ret = getIf(expr);
//     else
//         ret = getA(expr);
//
//     if (expr->tokens[expr->pos]->type != TYPE_AND)
//     {
//         SYNTAX_ERROR("Op must end with \";\"");
//     }
//     expr->pos++;
//
//     return ret;
// }

tree_node_t* getComp(expr_t* expr)
{
    tree_node_t* op1 = getOp(expr);

    while (expr->tokens[expr->pos]->type == TYPE_AND)
    {
        tree_node_t* connect = expr->tokens[expr->pos++];
        tree_node_t* op2 = getOp(expr);

        if (op2 == nullptr) return nullptr;

        connect->left = op1;
        op1->parent = connect;

        connect->right = op2;
        op2->parent = connect;

        op1 = connect;
    }

    // if (expr->tokens[expr->pos]->type != TYPE_AND)
    // {
    //     SYNTAX_ERROR("must be \";\" after comp"); return nullptr;
    // }
    // expr->pos++;

    return op1;
}

void error_message(expr_t* expr, const char* func, const char* message)
{
    ASSERT(log_file);

    fprintf(log_file, "<pre>Func: %s; ERROR: %s but found \"%s\", line = %lu, pos = %lu\n</pre>",
                        func, message, expr->tokens[expr->pos]->name, expr->tokens[expr->pos]->line + 1,
                        expr->tokens[expr->pos]->pos + 1);
}

int is_equal(double num1, double num2)
{
    ASSERT(isfinite(num1));
    ASSERT(isfinite(num2));

    return fabs(num1 - num2) < EPS;
}
