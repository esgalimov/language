#ifndef EXPR
#define EXPR

#include "../../lib/tree/tree.h"
#include "../../lib/tree/tree_debug.h"
#include "../../lib/text_funcs/text_funcs.h"
#include "dsl.h"

//! @brief Max len of func or var
const int NAME_MAX_LEN = 32;

//! @brief Max amount of ids
const int IDS_MAX_CNT  = 32;

//! @brief Max amount of tokens
const int TOKS_MAX_CNT = 1024;

//! @brief Const for return if there is no id
const int NO_ID        = -1;

//! @brief Poison
const size_t POISON    = 0xDEADBEEF;

//! @brief syntax of language
#define CH_ADD '+'
#define CH_SUB '-'
#define CH_MUL '*'
#define CH_DIV '/'
#define CH_LBR '('
#define CH_RBR ')'
#define CH_SEM ';'
#define CH_ASG '='

#define STR_DEF    "esh"
#define STR_RET    "bir"
#define STR_WHILE  "tugyaryak"
#define STR_IF     "egyar"
#define STR_ELSE   "bylmasa"
#define STR_PRINTF "yozyrga"
#define STR_SCANF  "ukyrga"
#define STR_BEGIN  "bashlau"
#define STR_END    "tuktau"

#define STR_ALLNUMS  "buten_sannar"
#define STR_NOROOTS "tomyrlar_uk"

#define STR_EQ  "<>"
#define STR_NEQ "><"
#define STR_GE  ">="
#define STR_G   ">"
#define STR_LE  "<="
#define STR_L   "<"


//! @brief Table of names (can be func or var)
//! @var name - name of var/func
//! @var type - type of id
typedef struct
{
    char*  name;
    node_type type;
} id_item_t;

//! @brief Struct to read programm text and make tokens (main struct in frontend)
//! @var program  - ptr to text of program maked with text funcs lib
//! @var pos      - pos in line (when start recursive descent use as pos in tokens)
//! @var line     - line where read
//! @var id_cnt   - counter in id table
//! @var ids      - name table
//! @var toks_cnt - counter of tokens
//! @var tokens   - array with ptrs to tokens (unlinked tree nodes)
//! @var tree     - ptr to syntax tree
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

//! @brief Constructor to expr
//! @param [in] filename - file with program
//! @return ptr to created expr struct, tokens and tree are created too
expr_t* expr_ctor(const char* filename);

//! @brief Destructor for expr
//! @return 0 - if Ok, 1 - else
int expr_dtor(expr_t* expr);

//! @brief Func to create tokens from program text
//! @param [in] expr - ptr to expr
//! @return 0 - if Ok, 1 - else
int create_tokens(expr_t* expr);

//! @brief Func to read name of function or variable
//! @param [in] expr - ptr to expression struct
//! @return ptr to name (must be freed after using)
char* read_name(expr_t* expr);

//! @brief Find id by its name
//! @param [in] expr - ptr to expr
//! @param [in] name - name to find
//! @return index of id if found, -1 - if not found
int find_id(expr_t* expr, const char* name);

//! @brief Create number from string
//! @param [in] expr - ptr to expr
//! @return read number (NAN if mistake)
double read_number(expr_t* expr);

//! @brief Func to dump all tokens before linking to tree
//! use graphiz funcs from tree_debug lib in it
//! @return 0 - if Ok
int tokens_dump(expr_t* expr);

//! @brief Create new token what have name more than one letter
//! @param [in] expr - ptr to expr
//! @param [in] type - type of node
//! @param [in] name - name
void new_word_token(expr_t* expr, node_type type, char* name);

//! @brief Create new token what have name of one word
//! @param [in] expr - ptr to expr
//! @param [in] type - type of node
//! @param [in] name - name
void new_char_token(expr_t* expr, node_type type, char ch);

//! @brief Make ast tree
//! @param [in] filename - name of prog file
//! Func create tree and write it in ast format
//! @return 0 - if Ok, 1 - else
int make_ast_tree(const char* filename);

//! @brief Func to print in ast format
//! @param [in] node - start node
//! @param [out] stream - ptr to file
void tree_print_preorder(tree_node_t* node, FILE* stream);

#endif
