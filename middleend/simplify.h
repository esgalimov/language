#ifndef SIMPLIFY
#define SIMPLIFY

#include "create_asm.h"


//! @brief Eval right child
#define evalR() eval(node->right)
//! @brief Eval left chuld
#define evalL() eval(node->left)

//! @brief Create node with value x
#define NUM(x) create_node(TYPE_NUM, old_node->line, old_node->pos, x)

//! @brief Compare node right child value and x
#define CMP_LEFT(x)  ((*node)->left->type  == TYPE_NUM && is_equal((*node)->left->value,  x))

//! @brief Compare node right child value and x
#define CMP_RIGHT(x) ((*node)->right->type == TYPE_NUM && is_equal((*node)->right->value, x))

//! @brief Func what count subtrees without variables
//! @param [in] node - ptr to start node
//! @return ptr to simplified subtree
void tree_eval_simplify(tree_node_t** node);

//! @brief Func what remove *1, /1, *0, +0, -0
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
//! @param [out] prog - ptr to programm struct
//! @param [out] node - ptr to ptr to node
void right_instead_node(prog_tree_t* prog, tree_node_t** node);

//! @brief Replace node with its left child
//! @param [out] prog - ptr to programm struct
//! @param [out] node - ptr to ptr to node
void left_instead_node(prog_tree_t* prog, tree_node_t** node);

//! @brief Replace node with given number
//! @param [out] prog - ptr to programm struct
//! @param [out] node - ptr to ptr to node
//! @param [in]  num  - number to replace
void num_instead_node(prog_tree_t* prog, tree_node_t** node, elem_t num);

//! @brief Replace node to its negative value
//! @param [out] prog - ptr to programm struct
//! @param [out] node - ptr to ptr to node
void neg_instead_node(prog_tree_t* prog, tree_node_t** node);

//! @brief Eval subtree if there are only nums ans operations
//! @param [in] node - subtree root
//! @return evaled value
double eval(const tree_node_t* node);

//! @brief Compare two nums with using EPS
//! @param [in] num1 - first number
//! @param [in] num2 - second number
//! @return 1 - equal, 0 - else
int is_equal(double num1, double num2);
//! @brief const using in func is_equal
const double EPS = 0.000001;
#endif
