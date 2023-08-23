#ifndef SIMPLIFY
#define SIMPLIFY

#include "create_asm.h"



#define evalR() eval(node->right)
#define evalL() eval(node->left)

#define NUM(x) create_node(TYPE_NUM, old_node->line, old_node->pos, x)

#define CMP_LEFT(x)  ((*node)->left->type  == TYPE_NUM && is_equal((*node)->left->value,  x))

#define CMP_RIGHT(x) ((*node)->right->type == TYPE_NUM && is_equal((*node)->right->value, x))

const double EPS = 0.000001;

//! @brief Func what count subtrees without variables
//! @param [in] node - ptr to start node
//! @return ptr to simplified subtree
void tree_eval_simplify(tree_node_t** node);

//! @brief Func what remove *1, /1, ˆ1, *0, ˆ0, 0ˆ, 1ˆ and count subtrees without variables
//! @param [in] node - ptr to start node
//! @param [in] prog - ptr to tree, it used to change 'simplify'
//! variable to check if tree stop changing and we must stop simplifying
//! @return ptr to simplified subtree
void tree_simplify(prog_tree_t* prog, tree_node_t** node);

//! @brief Func what check if there are variables in subtree
//! @param [in] node - ptr to node
//! @return 1 - there is(are) var(s), 0 - else
int can_eval(tree_node_t* node);

//! @brief Replace node with its right child
void right_instead_node(prog_tree_t* prog, tree_node_t** node);

//! @brief Replace node with its left child
void left_instead_node(prog_tree_t* prog, tree_node_t** node);

//! @brief Replace node with given number
void num_instead_node(prog_tree_t* prog, tree_node_t** node, elem_t num);

//! @brief Replace node to its negative value
void neg_instead_node(prog_tree_t* prog, tree_node_t** node);

double eval(const tree_node_t* node);

int is_equal(double num1, double num2);
#endif
