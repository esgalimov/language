#ifndef CPU
#define CPU

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define stack_ctor(stk, size) stack_ctor_((stk), (size), var_info {#stk, LOCATION})
#define stack_dump(stk, error_number) stack_dump_((stk), (error_number), __PRETTY_FUNCTION__, __FILE__, __LINE__)

typedef int elem;

struct var_info
{
    const char * name;
    const char * func;
    const char * file;
    int line;
};

struct stack
{
    elem * data;
    size_t size;
    size_t capacity;
    var_info info;
};

struct s_cpu
{
    stack stk;
    elem * cmd_buffer;
    elem ax;
    elem bx;
    elem cx;
    elem dx;
    elem * cpu_ram;
};

enum Errors
{
    NULL_DATA = 1,
    SIZE_ERROR = 2,
    CAP_ERROR = 4,
    SIZE_CAP_ERROR = 8,
    POP_ERROR = 16,
};

const size_t ERRORS_COUNT = 5;

enum Commands
{
    HLT = 0,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    DIV = 4,
    MUL = 5,
    POP = 6,
    OUT = 7,
    JMP = 8,
    JB = 9,
    JBE = 10,
    JA = 11,
    JAE = 12,
    JE = 13,
    JNE = 14,
    CALL = 15,
    RET = 16,
    AX = 17,
    BX = 18,
    CX = 19,
    DX = 20,
    PUSH_REG = 21,
    POP_REG = 22,
    IN = 23,
    SQRT = 24,
    NOROOTS = 25,
    ALLNUM = 26,
};

const int MIN_CAPACITY = 8;
const int ACCURACY = 100;

extern FILE * log_file;

int stack_ctor_(stack * stk, size_t capacity, var_info info);

int stack_verify(stack * stk);

void stack_push(stack * stk, elem value);

void stack_pop(stack * stk, elem * value);

void stack_resize(stack * stk, size_t new_size);

void error_num_translate(int error_number);

void stack_dump_(stack * stk, int error_number, const char * func, const char * file, int line);

void write_stack_elems(stack * stk);

void write_zeros_to_data(stack * stk, size_t i_start, size_t i_end);

void stack_dtor(stack * stk);

void test_stack(void);

int run_cpu(FILE * stream);

size_t cpu_ctor(s_cpu * cpu, FILE * stream);

void cpu_dtor(s_cpu * cpu);

#endif
