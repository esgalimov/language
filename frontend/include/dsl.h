//! @brief New token which name consist of several symbols
#define NEW_WORD_TOKEN(type) new_word_token(expr, type, name)

//! @brief New token which name consist of one symbol
#define NEW_CHAR_TOKEN(type) new_char_token(expr, type, CURR_CH)

//! @brief create num with value x (use in simplify)
#define NEW_NUM(x)           create_node(TYPE_NUM, expr->line, expr->pos, x)

//! @brief Current symbol at line
#define CURR_CH               expr->program->strings[expr->line][expr->pos]

//! @brief Print sintax error
#define SYNTAX_ERROR(message) error_message(expr, __PRETTY_FUNCTION__, message)


