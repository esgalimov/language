#ifndef CREATE_ASM
#define CREATE_ASM

#include "../lib/tree/tree.h"
#include "../lib/tree/tree_debug.h"


const int NAME_MAX_LEN = 32;
const int IDS_MAX_CNT  = 32;

#define tree_print_asm_l() tree_print_asm(node->left, prog, stream);
#define tree_print_asm_r() tree_print_asm(node->right, prog, stream);


const char LOCAL  = 0;
const char GLOBAL = 1;
const char PARAM_PART = 2;

typedef struct
{
    char* name;
    char  global;
} var_t;

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


tree_node_t* read_tree_preorder(prog_tree_t* prog);

size_t find_var(prog_tree_t* prog, char* name);

int read_ast_tree(void);

int translate_asm(prog_tree_t* prog);

void tree_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream);

void  var_print_asm(tree_node_t* node, prog_tree_t* prog, FILE* stream);

void pop_params_in_def(tree_node_t* node, prog_tree_t* prog, FILE* stream);

void push_params_in_func(tree_node_t* node, prog_tree_t* prog, FILE* stream);

int func_params_cnt(tree_node_t* node);

#endif
