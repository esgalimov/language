#ifndef ASM
#define ASM

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "text_funcs.h"
#include "DSL_check_code.h"

#define LOG_MODE

#ifdef LOG_MODE
    #define ASSERT(condition)                                                               \
        if (!(condition))                                                                   \
        {                                                                                   \
            printf("\nError in \"%s\" in %d line in function %s in file %s\n",              \
                    #condition, __LINE__, __PRETTY_FUNCTION__, __FILE__);                   \
            abort();                                                                        \
        }
    #define tree_dump(tree, expr) tree_dump_((tree), (expr), __PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
    #define ASSERT(condition)
#endif

typedef struct
{
    char * name;
    int    type;
    int    value;
    int    line;
} token_t;

typedef struct
{
    int    value;
    size_t cnt;
} label_t;

typedef struct
{
    text_t    commands;

    token_t*  toks;
    size_t    toks_cnt;

    token_t** label_toks;
    label_t*  labels;
    int       labels_cnt;

    int       len_cmd;
    int       len_cmd_gap;
} asm_t;

enum command_type
{
    UNDEFIND  = -5,
    LABEL_JMP = -4,
    LABEL     = -3,
    NUM_RAM   = -2,
    NUM       = -1,
    CMD0      =  0,
    CMD1      =  1,
    REG       =  2,
    REG_RAM   =  3,
};

enum commands
{
    HLT      =  0,
    PUSH     =  1,
    ADD      =  2,
    SUB      =  3,
    DIV      =  4,
    MUL      =  5,
    POP      =  6,
    OUT      =  7,
    JMP      =  8,
    JB       =  9,
    JBE      = 10,
    JA       = 11,
    JAE      = 12,
    JE       = 13,
    JNE      = 14,
    CALL     = 15,
    RET      = 16,
    AX       = 17,
    BX       = 18,
    CX       = 19,
    DX       = 20,
    PUSH_REG = 21,
    POP_REG  = 22,
    PUSH_RAM = 23,
    POP_RAM  = 24,
    IN       = 25,
    SQRT     = 26,
    NOROOTS  = 27,
    ALLNUM   = 28,
};

enum ram_cmd_types
{
    TYPE_NOT_RAM     = 0,
    TYPE_REG_RAM     = 1,
    TYPE_NUM_RAM     = 2,
    TYPE_NUM_REG_RAM = 3,
};

const int N_LABELS = 100;
const int ACCURACY = 100;
const int MAX_CMD_LEN = 32;

typedef int elem_t;

void run_comp(FILE * stream);

int str_of_digits(const char * cmd);

void write_code_to_file(token_t * toks, size_t n_cmd);

void check_toks_for_size(asm_t * asem, size_t i_code);

int check_code(asm_t * asem, size_t n_cmd);

int is_label(const char * cmd);

void asm_ctor(asm_t * asem, FILE * stream);

void asm_dtor(asm_t * asem);

//! @brief If after one of jump commands (jmp, jb, jbe, ja, jae, je, jne, call) locate label,
//! @brief function change label's type from LABEL to LABEL_JMP (type LABEL_JMP will be written into code array)
//! @brief If after pop or push locate register (type - REG), function change them type (PUSH -> PUSH_REG, POP -> POP_REG)
//! @param [in] asem - ptr to assembler struct
//! @param [in] n_toks - number of tokens

void make_label_jmp_push_reg(asm_t * asem, size_t n_toks);

//! @brief Check is token's value JMP, JB, JBE, JA, JAE, JE, JNE, CALL
//! @param [in] value - token's value
//! @return 1 if one of values in brief, else - 0

int check_for_jump(int value);

//! @brief Function for finding labels in tokens and count each one,
//! write label's value and number of it to labels array in assembler
//! @param [out] asem - pointer to assembler
//! @param [in] n_cmd - number of tokens

void labels_init(asm_t * asem, size_t n_toks);

//! @brief Find label
//! @param [in] asem - ptr to asm struct
//! @param [in] name - ptr to name to find
//! @return value - if exist, -1 - else
int find_label(asm_t* asem, const char* name);

//! @brief Check if command is connected with ram (like [35], [ax] or [27+ax])
//! @param [in] cmd - ptr to command string
//! @return 0 - if not ram, 1 - if like [35], 2 - if like [ax], 3 - if like [27+ax]
int is_ram(const char* cmd);

void print_error_message(int* is_ok, size_t i, asm_t* asem, const char* message);

#endif
