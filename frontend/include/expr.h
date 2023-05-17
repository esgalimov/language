#ifndef EXPR
#define EXPR

#include "../../lib/tree/tree.h"
#include "../../lib/tree/tree_debug.h"
#include "../../lib/text_funcs/text_funcs.h"
#include "dsl.h"


const int NAME_MAX_LEN = 32;
const int IDS_MAX_CNT  = 32;
const int TOKS_MAX_CNT = 1024;
const int NO_ID        = -1;
const size_t POISON = 0xDEADBEEF;


//! @brief id_t
typedef struct
{
    char * name;
    elem_t value;
    node_type type;
} id_item_t;

//! @brief Struct to read expression
typedef struct
{
    text_t*       program;
    size_t        pos;
    size_t        line;

    size_t        id_cnt;
    id_item_t**   ids;

    size_t        toks_cnt;
    tree_node_t** tokens;

    tree_t*       tree;
} expr_t;

//! @brief...
expr_t* expr_ctor(const char* filename);

//! @brief ...
int expr_dtor(expr_t* expr);

//! @brief
int create_tokens(expr_t* expr);

//! @brief Func to read name of function or variable
//! @param [in] expr - ptr to expression struct
//! @return ptr to name (must be freed after using)
char* read_name(expr_t* expr);

//! @brief Find var by its name
int find_id(expr_t* expr, const char* name);

//! @brief ...
double read_number(expr_t* expr);

//! @brief ...
int tokens_dump(expr_t* expr);

//! @brief ...
void new_word_token(expr_t* expr, node_type type, char* name);

//! @brief ...
void new_char_token(expr_t* expr, node_type type, char ch);

//! @brief ...
int make_ast_tree(const char* filename);

//! @brief ...
void tree_print_preorder(tree_node_t* node, FILE* stream);

#endif
