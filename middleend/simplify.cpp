#include "simplify.h"


double eval(const tree_node_t* node)
{
    if (node == nullptr)
    {
        fprintf(log_file, "<pre>Try to eval NULL node</pre>");
        return NAN;
    }

    switch (node->type)
    {
        case TYPE_NUM:
        {
            if (!isfinite(node->value))
                return NAN;
            return node->value;
        }
        case TYPE_VAR:
        {
            fprintf(log_file, "<pre>Try to eval TYPE_VAR node</pre>");
            return NAN;
        }
        case TYPE_ADD: return evalL() + evalR();
        case TYPE_SUB: return evalL() - evalR();
        case TYPE_MUL: return evalL() * evalR();
        case TYPE_DIV:
        {
            double eval_right = evalR();

            if (is_equal(eval_right, 0))
            {
                fprintf(log_file, "<pre>ERROR: Division by zero, %p</pre>", node);
                subtree_dump(node);
                return NAN;
            }
            return eval(node->left) / eval_right;
        }
        case TYPE_EQ:  return  is_equal(evalL(), evalR());
        case TYPE_NEQ: return !is_equal(evalL(), evalR());
        case TYPE_GE:  return evalL() >= evalR();
        case TYPE_G:   return evalL() >  evalR();
        case TYPE_LE:  return evalL() <= evalR();
        case TYPE_L:   return evalL() <  evalR();

        default:
            fprintf(log_file, "<pre>Can't eval</pre>");
            subtree_dump(node);
            return NAN;
    }
}

void tree_eval_simplify(tree_node_t** node)
{
    if (*node == nullptr) return;
    if (can_eval(*node) == 0)
    {
        tree_node_t * old_node = *node;
        *node = NUM(eval(*node));
        (*node)->parent = old_node->parent;
        nodes_dtor(old_node);
        return;
    }
    tree_eval_simplify(&((*node)->left));
    tree_eval_simplify(&((*node)->right));
}

void tree_simplify(prog_tree_t* prog, tree_node_t** node)
{
    if (*node == NULL) return;

    tree_eval_simplify(node);

    int old_simp = prog->simplify;

    if ((*node)->type == TYPE_MUL)
    {
        if (CMP_LEFT(0) || CMP_RIGHT(0)) num_instead_node(prog, node, 0);
        else if (CMP_LEFT(1))          right_instead_node(prog, node);
        else if (CMP_RIGHT(1))          left_instead_node(prog, node);
    }
    else if ((*node)->type == TYPE_ADD)
    {
        if      (CMP_LEFT(0))  right_instead_node(prog, node);
        else if (CMP_RIGHT(0))  left_instead_node(prog, node);
    }
    else if ((*node)->type == TYPE_SUB)
    {
        if      (CMP_RIGHT(0)) left_instead_node(prog, node);
        else if (CMP_LEFT(0))   neg_instead_node(prog, node);
    }
    else if ((*node)->type == TYPE_DIV)
    {
        if (CMP_RIGHT(0))
        {
            fprintf(log_file, "<pre>ERROR: Division by zero, %p</pre>", node);
            return;
        }
        if      (CMP_LEFT(0))    num_instead_node(prog, node, 0);
        else if (CMP_RIGHT(1))  left_instead_node(prog, node);
    }

    tree_simplify(prog, &((*node)->left));
    tree_simplify(prog, &((*node)->right));

    if (prog->simplify > old_simp)
        tree_simplify(prog, node);
}

void right_instead_node(prog_tree_t* prog, tree_node_t** node)
{
    tree_node_t* old_node = *node;

    *node = old_node->right;
    (*node)->parent = old_node->parent;
    prog->simplify++;

    free(old_node->left);
    free(old_node);
}

void left_instead_node(prog_tree_t* prog, tree_node_t** node)
{
    tree_node_t* old_node = *node;

    *node = old_node->left;
    (*node)->parent = old_node->parent;
    prog->simplify++;

    free(old_node->right);
    free(old_node);
}

void num_instead_node(prog_tree_t* prog, tree_node_t** node, elem_t num)
{
    tree_node_t* old_node = *node;

    *node = NUM(num);
    prog->simplify++;

    nodes_dtor(old_node);
}

void neg_instead_node(prog_tree_t* prog, tree_node_t** node)
{
    tree_node_t* old_node = *node;

    *node = create_node(TYPE_MUL, 0, 0, 0, NUM(-1), old_node->right);
    prog->simplify++;

    free(old_node->left);
    free(old_node);
}

int can_eval(tree_node_t* node)
{
    if (node == nullptr) return 0;

    if (!(TYPE_NUM <= node->type && node->type <= TYPE_L)) return 1;

    return can_eval(node->left) + can_eval(node->right);
}

int is_equal(double num1, double num2)
{
    ASSERT(isfinite(num1));
    ASSERT(isfinite(num2));

    return fabs(num1 - num2) < EPS;
}
