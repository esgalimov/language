#ifndef CPU
#define CPU

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

//! @brief Location of function
#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__

//! @brief Stack Constructor
#define stack_ctor(stk, size) stack_ctor_((stk), (size), var_info {#stk, LOCATION})

//! @brief Stack Dump
#define stack_dump(stk, error_number) stack_dump_((stk), (error_number), __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define LOG_MODE
//! My assert with more info
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

//! @brief Info about stack
//! @var name - stack name
//! @var func - func where stack is
//! @var file - file name
//! @var line - line where stack is
typedef struct
{
    const char* name;
    const char* func;
    const char* file;
    int         line;
} var_info;

//! @brief Stack
//! @var data - stack array
//! @var size - next index to push
//! @var capacity - size of data
//! @var info - info about stack
typedef struct
{
    elem_t*  data;
    size_t   size;
    size_t   capacity;
    var_info info;
} stack;

//! @brief Cpu struct
//! @var stk - main stack
//! @var stk_ret - stack for return indexes
//! @var cmd_buffer - commands to execute
//! @var ax, bx, cx, cd - registers
//! @var cpu_ram - 'random access memory'
typedef struct
{
    stack    stk;
    stack    stk_ret;
    elem_t*  cmd_buffer;
    elem_t   ax;
    elem_t   bx;
    elem_t   cx;
    elem_t   dx;
    elem_t*  cpu_ram;
} cpu_t;

//! @brief Stack error codes
enum Errors
{
    NULL_DATA      =  1 << 0,
    SIZE_ERROR     =  1 << 1,
    CAP_ERROR      =  1 << 2,
    SIZE_CAP_ERROR =  1 << 3,
    POP_ERROR      =  1 << 4,
};

//! @brief Amount of error codes
const size_t ERRORS_COUNT = 5;

//! @brief Cpu commands
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

//! @brief Min initial stack capacity
const int MIN_CAPACITY = 8;

//! @brief Max accuracy in cpu (nums store in stack and ram mulplied to ACCURACY)
const int ACCURACY = 100;

//! @brief RAM size
const int RAM_SIZE = 2048;

//! @brief Global ptr to log file
extern FILE* log_file;

//! @brief Stack constructor
//! @param [out] stk - ptr to stack struct
//! @param [in] capacity - initial size of data
//! @param [in] info - info about stack
//! @return 0 - if ok, 1 - else
int stack_ctor_(stack* stk, size_t capacity, var_info info);

//! @brief Check stack errors
//! @param [in] stk - ptr to stack
//! @return Summarize codes of mistakes to make number where each bit is concrete mistake and return it
int stack_verify(stack* stk);

//! @brief Stack push
//! @param [out] stk - ptr to stack struct
//! @param [in] value - num to push
void stack_push(stack* stk, elem_t value);

//! @brief Stack pop
//! @param [out] stk - ptr to stack struct
//! @param [in] value - ptr to num to pop
void stack_pop(stack* stk, elem_t* value);

//! @brief Stack resize
//! @param [out] stk - ptr to stack struct
//! @param [in] new_size - new capacity of stack
void stack_resize(stack* stk, size_t new_size);

//! @brief Write error text to log file
//! @param [in] error_number - error number made with stack_verify function
void error_num_translate(int error_number);

//! @brief Dump stack to log file
//! @param [in] stk - ptr to stack
//! @param [in] error_number - error number
//! @param [in] func - function where dump is
//! @param [in] file - file where dump is
//! @param [in] line - line where dump is
void stack_dump_(stack* stk, int error_number, const char* func, const char* file, int line);

//! @brief Print stack elems to log file when dump
//! @param [in] stk - ptr to stack struct
void write_stack_elems(stack* stk);

//! @brief Write zeros to data
//! @param [in] stk - ptr to stack struct
//! @param [in] i_start - index of start
//! @param [in] i_end - index of end
void write_zeros_to_data(stack* stk, size_t i_start, size_t i_end);

//! @brief Stack destructor
//! @param [in] stk - ptr to stack struct
void stack_dtor(stack* stk);

//! @brief Func to execute code
//! @param [in] stream - file with bin code
int run_cpu(FILE* stream);

//! @brief Get register pointer
//! @param [in] cpu - ptr to cpu struct
//! @param [in] i - index in buffer
//! @return ptr to register or nullptr if not register
elem_t* get_reg_ptr(cpu_t* cpu, size_t i);

//! @brief Cpu constructor
//! @param [in] cpu - ptr to cpu struct
//! @param [in] stream - file with bin code
//! @return number amount
size_t cpu_ctor(cpu_t* cpu, FILE* stream);

//! @brief Cpu constructor
//! @param [in] cpu - ptr to cpu struct
void cpu_dtor(cpu_t* cpu);

#endif
