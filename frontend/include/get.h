#ifndef GET
#define GET

#include "tree.h"
#include "tree_debug.h"
#include "expr.h"


//! @brief Get funcs
tree_node_t* getG(expr_t* expr);
tree_node_t* getN(expr_t* expr);
tree_node_t* getE(expr_t* expr);
tree_node_t* getT(expr_t* expr);
tree_node_t* getP(expr_t* expr);
tree_node_t* getA(expr_t* expr);
tree_node_t* getIf(expr_t* expr);
tree_node_t* getOp(expr_t* expr);
tree_node_t* getComp(expr_t* expr);
//! @brief Get Func or variable (varibles are only one-letter)
tree_node_t * getId(expr_t* expr);

//! @brief Func to write error message to log file
void error_message(expr_t* expr, const char* func, const char* message);

//! @brief Compare two nums with using EPS
int is_equal(double num1, double num2);
const double EPS = 0.000001;

#endif
