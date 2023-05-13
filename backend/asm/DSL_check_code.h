#define IS_UNDEFIND(i) (asem->toks[i].type  == UNDEFIND)

#define IS_CMD1(i)     (asem->toks[i].type  == CMD1)

#define IS_CMD0(i)     (asem->toks[i].type  == CMD0)

#define IS_PUSH(i)     (asem->toks[i].value == PUSH)

#define IS_POP(i)      (asem->toks[i].value == POP)

#define IS_NUM(i)      (asem->toks[i].type  == NUM)

#define IS_NUM_RAM(i)  (asem->toks[i].type  == NUM_RAM)

#define IS_REG(i)      (asem->toks[i].type  == REG)

#define IS_REG_RAM(i)  (asem->toks[i].type  == REG_RAM)

#define IS_LABEL(i)    (asem->toks[i].type  == LABEL)

#define IS_HLT(i)      (asem->toks[i].value  == HLT)

#define ERROR_MESSAGE(message) print_error_message(&is_ok, i, asem, message)
