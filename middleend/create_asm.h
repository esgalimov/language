#ifndef CREATE_ASM
#define CREATE_ASM

#include "../lib/tree/tree.h"
#include "../lib/tree/tree_debug.h"


//! @brief Max len of name in AST
const int NAME_MAX_LEN = 32;

//! @brief Max amount of ids (vars and funcs)
const int IDS_MAX_CNT  = 32;

//! @brief Var in local space
const char LOCAL  = 0;

//! @brief Var in global space
const char GLOBAL = 1;

//! @brief Defines to print left and right child as asm file
#define tree_print_asm_l() tree_print_asm(node->left, prog, stream);
#define tree_print_asm_r() tree_print_asm(node->right, prog, stream);

//! @brief Var struct
//! @var name - var name
//! @var global - LOCAL or GLOAL space
typedef struct
{
    char* name;
    char  global;
} var_t;


//! @brief Programm struct
//! @var tree - ptr to tree made from .ast file
//! @var vars - var table
//! @var var_cnt - amount of vars
//! @var buffer - place to read .ast file
//! @var pos - position in .ast file to read it preorder
//! @var simplify - amount of simplifies for one call symplify function, if this var don't change, func stop simpifies
//! @var part - GLOBAL or LOCAL space while read preorder
typedef struct
{
    tree_t* tree;

    var_t*  vars;
    size_t  var_cnt;

    char*   buffer;
    size_t  pos;
    int     simplify;
    char    part;
} prog_tree_t;

//! @brief Make tree from .ast file
//! @param [out] prog - ptr to programm struct
//! @return node of syntax tree
tree_node_t* read_tree_preorder(prog_tree_t* prog);

//! @brief Find var in var table or make new var
//! @param [out] prog - ptr to prog struct
//! @param [in]  name - name to find
//! use global field in existing vars and part field in prog to decide var is local or global
//! @return index of var
size_t find_var(prog_tree_t* prog, char* name);

//! @brief Func controller to make asm from .ast file
//! @return 0 - Ok, 1 - else
int read_ast_tree(void);

//! @brief Func controller to make asm from tree
//! @param [out] prog - ptr to programm struct
//! @return 0 - Ok, 1 - else
int translate_asm(prog_tree_t* prog);

//! @brief Print tree like asm file
//! @param [in] node - ptr to current node
//! @param [out] prog - ptr to programm struct
//! @param [out] stream - ptr to asm file
void tree_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream);

//! @brief Print local vars as 'push [num+dx]' and global like 'push [num]'
//! @param [in] node - ptr to current node
//! @param [out] prog - ptr to programm struct
//! @param [out] stream - ptr to asm file
void  var_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream);

//! @brief Pop params to ram in the begining of function
//! @param [in] node - ptr to current node
//! @param [out] prog - ptr to programm struct
//! @param [out] stream - ptr to asm file
void pop_params_in_def(tree_node_t* node, prog_tree_t* prog, FILE* stream);

//! @brief Push param before function call
//! @param [in] node - ptr to current node
//! @param [out] prog - ptr to programm struct
//! @param [out] stream - ptr to asm file
void push_params_in_func(tree_node_t* node, prog_tree_t* prog, FILE* stream);

#endif
