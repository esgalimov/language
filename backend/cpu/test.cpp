#include "cpu.h"

void test_stack(void)
{
    stack stk1 = {};
    elem x = 0;
    stack_ctor(&stk1, 5);

    stack_push(&stk1, 1);
    stack_push(&stk1, 2);
    stack_push(&stk1, 3);
    stk1.capacity = 1;
    stk1.data = NULL;
    stack_push(&stk1, 4);

    stack_pop(&stk1, &x);
    stack_pop(&stk1, &x);
    stack_pop(&stk1, &x);
    stack_pop(&stk1, &x);
    stack_pop(&stk1, &x);
    // stk1.data = NULL;

    stack_dtor(&stk1);
}
