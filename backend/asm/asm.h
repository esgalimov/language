#ifndef ASM
#define ASM

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "../../lib/text_funcs/text_funcs.h"
#include "DSL_check_code.h"

#define LOG_MODE

//! @brief My assert with more info
#ifdef LOG_MODE
    #define ASSERT(condition)                                                               \
        if (!(condition))                                                                   \
        {                                                                                   \
            printf("\nError in \"%s\" in %d line in function %s in file %s\n",              \
                    #condition, __LINE__, __PRETTY_FUNCTION__, __FILE__);                   \
            abort();                                                                        \
        }
#else
    #define ASSERT(condition)
#endif

typedef int elem_t;

//! @brief Token struct
//! @var name - token name
//! @var type - token type (one of command_type)
//! @var value - token value (one of commands or other, for example, number value, index to jump)
//! @var line - command line
typedef struct
{
    char*  name;
    int    type;
    int    value;
    int    line;
} token_t;

//! @brief Label struct
//! @var value - index to jmp
//! @var cnt - number of this particular labes to check if the are more then one same labels
typedef struct
{
    int    value;
    size_t cnt;
} label_t;

//! @brief Main asm struct
//! @var commands - struct with asm code text, from text_funcs lib
//! @var toks - array of tokens
//! @var toks_cnt - amount of tokens
//! @var label_toks - array of only label tokens
//! @var label_cnt - amount of labels
//! @var len_cmd - cmd len (amount of symbols)
//! @var len_cmd_gap - var to save len_cmd for next iteration
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

//! @brief General command types
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

//! @brief Smaller command types
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

//! @brief Work with ram
//! TYPE_NOT_RAM: like 'push' or 'pop''
//! TYPE_NUM_RAM: like 'push [num]' or 'pop [num]'
//! TYPE_NUM_REG_RAM: like 'push [num+reg]' or 'pop [num+reg]'
enum ram_cmd_types
{
    TYPE_NOT_RAM     = 0,
    TYPE_NUM_RAM     = 1,
    TYPE_NUM_REG_RAM = 2,
};

//! @brief Max labels amount
const int N_LABELS = 100;

//! @brief Max accuracy in cpu (nums store in stack and ram mulplied to ACCURACY)
const int ACCURACY = 100;

//! @brief Max command length
const int MAX_CMD_LEN = 32;

//! @brief Func controller
//! @param [out] stream - file with asm code
void run_comp(FILE* stream);

//! @brief If string can be interpreted as number
//! @param [in] cmd - command
//! @return 1 - number, 0 - else
int str_of_digits(const char* cmd);

//! @brief Write cpu code to files
//! @param [in] toks - tokens
//! @param [in] n_cmd - amount of commands
//! write to two files, first is .bin to use in cpu, second is .txt to debug
void write_code_to_file(token_t* toks, size_t n_cmd);

//! @brief Check if toks array must be realloced
//! @param [out] asem - asm main struct ptr
//! @param [in] i_code - last token index
void check_toks_for_size(asm_t* asem, size_t i_code);

//! @brief Check code to mistakes and write them in console
//! @param [in] asm - ptr to asm struct
//! @param [in] n_cmd - number of commands
//! @return 1 - Ok, 0 - else
int check_code(asm_t* asem, size_t n_cmd);

//! @brief Check if token can be interpreted as label
//! @param [in] cmd - command
//! @return 1 - label, 0 - else
int is_label(const char* cmd);

//! @brief Asm constructor
//! @param [out] asem - asm struct ptr
//! @param [out] stream - asm file
void asm_ctor(asm_t* asem, FILE* stream);

//! @brief Asm destructor
//! @param [out] asem - asm struct ptr
void asm_dtor(asm_t* asem);

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

//! @brief Check if command is connected with ram (like [35] or [27+ax])
//! @param [in] cmd - ptr to command string
//! @return 0 - if not ram, 1 - if like [35], 2 - if like [27+ax]
int is_ram(const char* cmd);

//! @brief Print error message and is_ok = 0
//! @param [out] is_ok - ptr to is_ok that will be returned in check_code function
//! @param [in] i - token index
//! @param [in] asem - ptr to asm struct
//! @param [in] message - error message
void print_error_message(int* is_ok, size_t i, asm_t* asem, const char* message);

#endif
