#define NEW_WORD_TOKEN(type) new_word_token(expr, type, name)
#define NEW_CHAR_TOKEN(type) new_char_token(expr, type, CURR_CH)
#define NEW_NUM(x)           (expr->tokens[expr->toks_cnt++] = create_node(TYPE_NUM, expr->line, expr->pos, x))
#define CURR_CH               expr->program->strings[expr->line][expr->pos]


