#ifndef MY_TREE
#define MY_TREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define tree_ctor(tree) tree_ctor_((tree), var_info {#tree, LOCATION})

typedef double elem_t;

//! @brief Types of node
enum node_type
{
    TYPE_NUM,
    TYPE_ADD,
    TYPE_SUB,
    TYPE_MUL,
    TYPE_DIV,
    TYPE_EQ,
    TYPE_NEQ,
    TYPE_GE,
    TYPE_G,
    TYPE_LE,
    TYPE_L,

    TYPE_ASSIG,
    TYPE_ID,
    TYPE_VAR,

    TYPE_DEF,
    TYPE_FUNC,
    TYPE_RET,
    TYPE_WHILE,
    TYPE_IF,
    TYPE_ELSE,
    TYPE_PRINTF,
    TYPE_SCANF,

    TYPE_BEGIN,
    TYPE_END,
    TYPE_L_BR,
    TYPE_R_BR,
    TYPE_AND,

    TYPE_ALL,
    TYPE_NO,
};

//! @brief Link modes
enum link_mode
{
    LEFT  = 0,
    RIGHT = 1,
};

//! @struct var_info
//! @brief Information about tree to write to log
//! @var name - name of tree
//! @var func - function where tree created
//! @var file - file where tree created
//! @var line - line where tree created
typedef struct
{
    const char* name;
    const char* func;
    const char* file;
    int         line;
} var_info;

//! @brief Tree node struct
//! @var type  - type of node (operation of number)
//! @var valur - value of node
//! @var left  - ptr to left child
//! @var right - ptr to right child
typedef struct tree_node
{
    char*      name;
    int        type;
    elem_t     value;
    size_t     line;
    size_t     pos;
    tree_node* left;
    tree_node* right;
    tree_node* parent;
} tree_node_t;

//! @brief Tree struct
//! @var root   - root node ptr
//! @var status - tree status
//! @var info   - struct with info about tree
typedef struct
{
    tree_node_t* root;
    int          status;
    var_info     info;
} tree_t;

//! @brief Tree constructor
//! @param tree - ptr to tree
//! @return 0 - Ok, 1 - else
int tree_ctor_(tree_t* tree, var_info info);

//! @brief Tree destructor
//! @param tree - ptr to tree
//! @return 0 - Ok
int tree_dtor(tree_t* tree);

//! @brief Nodes destructor
//! @param [in] node - ptr to node
void nodes_dtor(tree_node_t* node);

//! @brief Link root node to tree
//! @param [in] tree - ptr to tree
//! @param [in] node - ptr to node
//! @return 0 - Ok, 1 - root is NULL
int link_root(tree_t* tree, tree_node_t* root);

//! @brief Create node
//! @param [in] type  - type of node
//! @param [in] value - value of node
//! @return ptr to node - if Ok, NULL - else
tree_node_t* create_node(node_type type, size_t line, size_t pos, elem_t value = NAN, tree_node_t* left = NULL, tree_node_t* right = NULL);

//! @brief Create diffenrent types of nodes, this funcs use create_node func
// tree_node_t* create_num(elem_t value);
// tree_node_t* create_op(node_type oper, tree_node_t* left = NULL, tree_node_t* right = NULL);
// tree_node_t* create_var(int value);
// tree_node_t* create_func(node_type func, tree_node_t* left = NULL, tree_node_t* right = NULL);


//! @brief Link node
//! @param [in] parent - ptr to parent node
//! @param [in] child  - ptr to node to link
//! @param [in] mode   - left or right child
//! @return 0 - Ok, 1 - else
int link_node(tree_node_t* parent, tree_node_t* child, link_mode mode);

//! @brief Copy subtree
//! @param [in] node - ptr to root of subtree
//! @return root of subtree copy
tree_node_t* copy_subtree(tree_node_t* node);

#endif
