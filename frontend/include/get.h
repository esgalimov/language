#ifndef GET
#define GET

#include "../../lib/tree/tree.h"
#include "../../lib/tree/tree_debug.h"
#include "expr.h"


//! @brief Get funcs
tree_node_t* getG(expr_t* expr);           // General
tree_node_t* getN(expr_t* expr);           // Number
tree_node_t* getE(expr_t* expr);           // Expression
tree_node_t* getEs(expr_t* expr);          // Expressions
tree_node_t* getT(expr_t* expr);           // * or /
tree_node_t* getId(expr_t* expr);          // Id
tree_node_t* getIds(expr_t* expr);         // Id; Id; ...
tree_node_t* getP(expr_t* expr);           // (...)
tree_node_t* getA(expr_t* expr);           // Assigment
tree_node_t* getIf(expr_t* expr);          // If of while
tree_node_t* getElse(expr_t* expr);        // Else
tree_node_t* getPrintfScanf(expr_t* expr); // Printf or Scanf
tree_node_t* getDef(expr_t* expr);         // Define of function
tree_node_t* getFuncCall(expr_t* expr);    // Call
tree_node_t* getFunc(expr_t* expr);        // Define, call or printf/scanf
tree_node_t* getComp(expr_t* expr);        // operator; operator; ....
tree_node_t* getOp(expr_t* expr);          // func, if, def, printf, scanf, while and other operators

//! @brief Func to write error message to log file
void error_message(expr_t* expr, const char* func, const char* message);

//! @brief Compare two nums with using EPS
int is_equal(double num1, double num2);
const double EPS = 0.000001;

#endif
