#define NEW_WORD_TOKEN(type) new_word_token(expr, type, name)

#define NEW_CHAR_TOKEN(type) new_char_token(expr, type, CURR_CH)

#define NEW_NUM(x)           create_node(TYPE_NUM, expr->line, expr->pos, x)

#define CURR_CH               expr->program->strings[expr->line][expr->pos]

#define SYNTAX_ERROR(message) error_message(expr, __PRETTY_FUNCTION__, message)


