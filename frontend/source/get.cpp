#include "../../lib/tree/tree.h"
#include "../../lib/tree/tree_debug.h"
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
    tree_node_t* comp = getComp(expr);

    if (comp != nullptr && expr->tokens[expr->pos]->type != TYPE_END)
    {
        SYNTAX_ERROR("prog must end with \"tuktau\""); return nullptr;
    }

    return comp;
}

tree_node_t* getE(expr_t* expr)
{
    tree_node_t* val1 = getT(expr);
    while (expr->tokens[expr->pos]->type == TYPE_ADD || expr->tokens[expr->pos]->type == TYPE_SUB)
    {
        tree_node_t* oper = expr->tokens[expr->pos++];
        tree_node_t * val2 = getT(expr);

        if (val1 == nullptr || val2 == nullptr) return nullptr;

        oper->left = val1;
        val1->parent = oper;

        oper->right = val2;
        val2->parent = oper;

        val1 = oper;
    }
    return val1;
}

tree_node_t* getEs(expr_t* expr)
{
    tree_node_t* ex1 = getE(expr);

    if (ex1 == nullptr) return nullptr;

    while (expr->tokens[expr->pos]->type == TYPE_AND)
    {
        tree_node_t* connect = expr->tokens[expr->pos++];
        tree_node_t* ex2 = getE(expr);

        if (ex2 == nullptr) return nullptr;

        connect->left = ex1;
        ex1->parent = connect;

        connect->right = ex2;
        ex2->parent = connect;

        ex1 = connect;
    }

    return ex1;
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
    {
        tree_node_t* id = getId(expr);

        if (id == nullptr) return nullptr;

        if (expr->ids[(int) id->value]->type == TYPE_VAR)
        {
            id->type = TYPE_VAR;
            return id;
        }
        else if (expr->ids[(int) id->value]->type == TYPE_FUNC)
        {
            expr->pos--;
            return getFuncCall(expr);
        }
        else
        {
            SYNTAX_ERROR("expected existing var"); return nullptr;
        }
    }
}

tree_node_t* getId(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type == TYPE_ID  ||
        expr->tokens[expr->pos]->type == TYPE_VAR ||
        expr->tokens[expr->pos]->type == TYPE_FUNC)
    {
        return expr->tokens[expr->pos++];
    }
    else
    {
        SYNTAX_ERROR("expected TYPE_ID, TYPE_VAR or TYPE_FUNC"); return nullptr;
    }
}

tree_node_t* getIds(expr_t* expr)
{
    tree_node_t* var1 = getId(expr);

    if (var1 == nullptr) return nullptr;

    expr->ids[(int) var1->value]->type = TYPE_VAR;
    var1->type = TYPE_VAR;

    while (expr->tokens[expr->pos]->type == TYPE_AND)
    {
        tree_node_t* connect = expr->tokens[expr->pos++];
        tree_node_t* var2 = getId(expr);

        if (var2 == nullptr) return nullptr;

        expr->ids[(int) var2->value]->type = TYPE_VAR;
        var2->type = TYPE_VAR;

        connect->left = var1;
        var1->parent = connect;

        connect->right = var2;
        var2->parent = connect;

        var1 = connect;

    }
    return var1;
}

tree_node_t* getN(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type == TYPE_SUB)
        expr->tokens[expr->pos++]->value *= -1;

    return expr->tokens[expr->pos++];
}

tree_node_t* getA(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type != TYPE_ID)
    {
        SYNTAX_ERROR("expected TYPE_ID"); return nullptr;
    }
    tree_node_t* var = expr->tokens[expr->pos++];

    if (expr->tokens[expr->pos]->type != TYPE_ASSIG)
    {
        SYNTAX_ERROR("expected TYPE_ASSIG"); return nullptr;
    }

    tree_node_t* assig = expr->tokens[expr->pos++];
    tree_node_t* value = getE(expr);

    if (value == nullptr) return nullptr;

    var->type = TYPE_VAR;
    assig->left = var;
    assig->right = value;

    var->parent = assig;
    value->parent = assig;

    expr->ids[(int) var->value]->type = TYPE_VAR;

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

    if (cond->type == TYPE_IF && expr->tokens[expr->pos]->type == TYPE_AND &&
                                 expr->tokens[expr->pos + 1]->type == TYPE_ELSE)
    {
        tree_node_t* and_node = expr->tokens[expr->pos++];

        tree_node_t* else_node = getElse(expr);

        if (else_node == nullptr) return nullptr;

        and_node->left = comp;
        comp->parent = and_node;

        and_node->right = else_node;
        else_node->parent = and_node;

        cond->right = and_node;
        and_node->parent = cond;
    }

    else
    {
        cond->right = comp;
        comp->parent = cond;
    }

    return cond;
}

tree_node_t* getElse(expr_t* expr)
{
    tree_node_t* else_tok = expr->tokens[expr->pos];

    if (else_tok->type != TYPE_ELSE)
    {
        SYNTAX_ERROR("expected TYPE_ELSE"); return nullptr;
    }

    expr->pos++;

    if (expr->tokens[expr->pos]->type != TYPE_BEGIN)
    {
        SYNTAX_ERROR("must be \"bashlau\" in start of else body"); return nullptr;
    }

    expr->pos++;
    tree_node_t* comp = getComp(expr);

    if (comp == nullptr) return nullptr;

    if (expr->tokens[expr->pos]->type != TYPE_END)
    {
        SYNTAX_ERROR("must be \"tuktau\" in end of else body"); return nullptr;
    }
    expr->pos++;

    else_tok->left = comp;
    comp->parent = else_tok;

    return else_tok;
}

tree_node_t* getFunc(expr_t* expr)
{
    tree_node_t* tok = expr->tokens[expr->pos];

    if (tok->type == TYPE_PRINTF || tok->type == TYPE_SCANF)
    {
        return getPrintfScanf(expr);
    }
    else if (tok->type == TYPE_DEF)
    {
        return getDef(expr);
    }
    else if (tok->type == TYPE_ID)
    {
        return getFuncCall(expr);
    }
    else
    {
        SYNTAX_ERROR("expected func"); return nullptr;
    }
}

tree_node_t* getPrintfScanf(expr_t* expr)
{
    tree_node_t* tok = expr->tokens[expr->pos];

    expr->pos++;
    if (expr->tokens[expr->pos]->type != TYPE_L_BR)
    {
        SYNTAX_ERROR("must be \"(\" aftre yozyrga/ukyrga"); return nullptr;
    }
    expr->pos++;

    tree_node_t* id = getId(expr);//---------------

    if (id == nullptr) return nullptr;

    if (expr->ids[(int) id->value]->type == TYPE_VAR)
    {
        id->type = TYPE_VAR;
    }
    else
    {
        SYNTAX_ERROR("expected existing var in yozyrga/ukyrga"); return nullptr;
    }
    if (expr->tokens[expr->pos]->type != TYPE_R_BR)
    {
        SYNTAX_ERROR("must be \")\" aftre printf or scanf"); return nullptr;
    }
    expr->pos++;

    tok->left = id;
    id->parent = tok;

    return tok;
}

tree_node_t* getDef(expr_t* expr)
{
    expr->pos++;
    tree_node_t* func = getId(expr);

    if (expr->tokens[expr->pos]->type != TYPE_L_BR)
    {
        SYNTAX_ERROR("must be \"(\" aftre printf or scanf"); return nullptr;
    }
    expr->pos++;

    tree_node_t* id = getIds(expr);

    if (id == nullptr) return nullptr;

    if (expr->tokens[expr->pos]->type != TYPE_R_BR)
    {
        SYNTAX_ERROR("must be \")\" aftre printf or scanf"); return nullptr;
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

    func->type = TYPE_DEF;

    func->left = id;
    id->parent = func;

    func->right = comp;
    comp->parent = func;

    expr->ids[(int) func->value]->type = TYPE_FUNC;

    return func;
}

tree_node_t* getReturn(expr_t* expr)
{
    tree_node_t* tok = expr->tokens[expr->pos];

    if (tok->type != TYPE_RET) return nullptr;

    expr->pos++;

    tree_node_t* ret_value = getE(expr);

    tok->left = ret_value;
    ret_value->parent = tok;

    return tok;
}

tree_node_t* getFuncCall(expr_t* expr)
{
    tree_node_t* tok = expr->tokens[expr->pos];

    if (expr->ids[(int) tok->value]->type != TYPE_FUNC)
    {
        SYNTAX_ERROR("expected existing func"); return nullptr;
    }
    expr->pos++;

    if (expr->tokens[expr->pos]->type != TYPE_L_BR)
    {
        SYNTAX_ERROR("expected \"(\" after func"); return nullptr;
    }
    expr->pos++;
    tree_node_t* expression = getEs(expr);

    if (expression == nullptr) return nullptr;

    if (expr->tokens[expr->pos]->type != TYPE_R_BR)
    {
        SYNTAX_ERROR("expected \")\" after func"); return nullptr;
    }
    expr->pos++;

    tok->type = TYPE_FUNC;

    tok->left = expression;
    expression->parent = tok;

    return tok;
}

tree_node_t* getOp(expr_t* expr)
{
    if (expr->tokens[expr->pos]->type == TYPE_IF || expr->tokens[expr->pos]->type == TYPE_WHILE)
        return getIf(expr);

    if (expr->tokens[expr->pos]->type == TYPE_DEF || expr->tokens[expr->pos]->type == TYPE_SCANF ||
        expr->tokens[expr->pos]->type == TYPE_PRINTF)
        return getFunc(expr);

    if (expr->pos < expr->toks_cnt - 1 && expr->tokens[expr->pos]->type == TYPE_ID &&
        expr->tokens[expr->pos + 1]->type == TYPE_L_BR)
        return getFunc(expr);

    if (expr->tokens[expr->pos]->type == TYPE_RET)
        return getReturn(expr);

    return getA(expr);
}

tree_node_t* getComp(expr_t* expr)
{
    tree_node_t* op1 = getOp(expr);

    while (expr->tokens[expr->pos]->type == TYPE_AND)
    {
        tree_node_t* connect = expr->tokens[expr->pos++];
        tree_node_t* op2 = getOp(expr);

        if (op2 == nullptr || op1 == nullptr) return nullptr;

        connect->left = op1;
        op1->parent = connect;

        connect->right = op2;
        op2->parent = connect;

        op1 = connect;
    }

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
