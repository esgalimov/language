#ifndef DEBUG_TREE
#define DEBUG_TREE

#include "tree.h"
#include "expr.h"

#define LOG_MODE

#ifdef LOG_MODE
    #define ASSERT(condition)                                                               \
        if (!(condition))                                                                   \
        {                                                                                   \
            fprintf(log_file, "\nError in \"%s\" in %d line in function %s in file %s\n",   \
                    #condition, __LINE__, __PRETTY_FUNCTION__, __FILE__);                   \
            abort();                                                                        \
        }
    #define tree_dump(tree, expr) tree_dump_((tree), (expr), __PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
    #define ASSERT(condition)
    #define tree_dump(tree, expr)
#endif

#define BLUE       "#87CEEB"
#define D_BLUE     "#0000CD"
#define L_BLUE     "#AFEEEE"
#define RED        "#FFD0D0"
#define L_RED      "#FCCACA"
#define D_RED      "#DC143C"
#define PINK       "#E6E6FA"
#define L_PINK     "#FFE3FF"
#define YELLOW     "#F0E68C"
#define L_YELLOW   "#FFFDC2"
#define GREEN      "#98FB98"
#define L_GREEN    "#E7F8BC"
#define ORANGE     "#FFE4C4"


#define OPERATIONS "+-*/"

//#define FUNCS "SIN", "COS", "LN", "POW", "LOG", "EXP"
const char FUNCS[6][10] = {"SIN", "COS", "LN", "^", "LOG", "EXP"};

//! @brief Errors that can be with tree
enum STATUS
{
    STATUS_OK                = 0 << 0,
    NUM_NODE_HAS_LEFT_CHILD  = 1 << 0,
    NUM_NODE_HAS_RIGHT_CHILD = 1 << 1,
    NODE_CREATE_ERROR        = 1 << 2,
    NODE_LINK_ERROR          = 1 << 3,
    MODE_LINK_ERROR          = 1 << 4,
    LINK_NULL_ROOT           = 1 << 5,
    ROOT_HAVE_PARENT         = 1 << 6,
    WRONG_PARENT             = 1 << 7,
    LEFT_RIGHT_SAME          = 1 << 8,
    NOT_ROOT_HAVE_NO_PARENT  = 1 << 9,
    CHILD_ITSELF             = 1 << 10,
    PARENT_ITSELF            = 1 << 11,
};

const int ERRORS_COUNT = 12;

//! @brief File to generate graphviz picture
extern FILE * graphviz_file;

//! @brief File to logs
extern FILE * log_file;

//! @brief Dump counter to make pictures names
extern int dump_cnt;

//! @brief Func to open graphiz_file
int open_graphviz_file(void);

//! @brief Func to close graphiz_file
//! write "}"before closing to finish making graphiz file
int close_graphviz_file(void);

//! @brief Func to open log_file and write some html tags to start logging
int open_log_file(void);

//! @brief Func to close log_file
//! write "</html>" before closing to finish making log file
int close_log_file(void);

//! @brief Graphiz file init
//! @param [in] list - ptr to tree's struct
int graphviz_init(tree_t* tree);

//! @brief Add nodes into .dot file to use graphiz
//! @param [in] node - ptr to node
void add_nodes(const tree_node_t* node, expr_t* expr);

//! @brief Link nodes in .dot file
//! @param [in] node - ptr to node
void link_nodes_gr(const tree_node_t* node);

//! @brief Tree dump
//! Make .dot file using funcs above and use graphiz to make png picture of dump
//! @param [in] list - ptr to tree
//! @param [in] func - ptr to func name
//! @param [in] file - ptr to file name
//! @param [in] line - line where was dump
//! @return 0
int tree_dump_(tree_t* tree, expr_t* expr, const char* func, const char* file, int line);

//! @brief Verify tree, create number
//! Summarize codes of mistakes to make number where each bit is concrete mistake
//! @param [in] list - ptr to tree
//! @return error number
int tree_verify(tree_t* tree);

//! @brief Verify node
//! @param [in] node - ptr to node
//! @return error number
void node_verify(tree_t * tree, const tree_node_t * node);

//! @brief Write to log errors with using error number
//! @param [in] error_number - error number what return tree verify
void error_number_translate(tree_t * tree);

//! @brief Dump subtree (part of tree from particular node)
int subtree_dump(const tree_node_t* node);

// //! @brief Check access to pointer
// //! use fwrite to try write one byte from pointer
// //! @param [in] ptr - pointer to check
// //! @return 1 - if OK, else - if error
// int check_ptr_access(const void * ptr);


#endif
