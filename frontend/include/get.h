#ifndef GET
#define GET

#include "tree.h"
#include "tree_debug.h"
#include "expr.h"


const int NO_VAR = -1;

//! @brief Get funcs
tree_node_t * getG(expr_t* expr);
tree_node_t * getN(expr_t* expr);
tree_node_t * getE(expr_t* expr);
tree_node_t * getT(expr_t* expr);
tree_node_t * getP(expr_t* expr);
tree_node_t * getD(expr_t* expr);
//! @brief Get Func or variable (varibles are only one-letter)
tree_node_t * getW(expr_t* expr);

//! @brief Func to read name of function or variable
//! @param [in] expr - ptr to expression struct
//! @return ptr to name (must be freed after using)
char * read_name(expr_t* expr);

//! @brief Find var by its name
int find_var(expr_t* expr, const char* name);


//! @brief Compare two nums with using EPS
int is_equal(double num1, double num2);
const double EPS = 0.000001;

#endif
