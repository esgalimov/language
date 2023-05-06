#include "cpu.h"

int run_cpu(FILE * stream)
{
    assert(stream != NULL);

    s_cpu cpu = {};
    size_t n_cmd = cpu_ctor(&cpu, stream);

    double num = NAN;
    elem num1 = 0, num2 = 0;

    //double d_num1 = NAN, d_num2 = NAN;


    for (size_t i = 0; i < n_cmd; i++)
    {
        switch (cpu.cmd_buffer[i])
        {
        case PUSH:
            stack_push(&cpu.stk, cpu.cmd_buffer[++i]);
            break;

        case ADD:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);
            stack_push(&cpu.stk, num1 + num2);
            break;

        case SUB:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);
            stack_push(&cpu.stk, num2 - num1);
            break;

        case DIV:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);
            stack_push(&cpu.stk, (elem) (((double) num2 / (double) num1) * ACCURACY));
            break;

        case MUL:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);
            stack_push(&cpu.stk, (num2 * num1) / ACCURACY);
            break;

        case POP:
            stack_pop(&cpu.stk, &num1);
            break;

        case OUT:
            stack_pop(&cpu.stk, &num1);
            printf("OUT: %.2lf\n", (double) num1 / ACCURACY);
            break;

        case JMP:
            i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            break;

        case JB:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);

            if (num2 < num1)
            {
                i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            }
            else i++;

            break;

        case JBE:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);

            if (num2 <= num1)
            {
                i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            }
            else i++;

            break;

        case JA:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);

            if (num2 > num1)
            {
                i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            }
            else i++;

            break;

        case JAE:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);

            if (num2 >= num1)
            {
                i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            }
            else i++;

            break;

        case JE:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);

            if (num2 == num1)
            {
                i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            }
            else i++;

            break;

        case JNE:
            stack_pop(&cpu.stk, &num1);
            stack_pop(&cpu.stk, &num2);

            if (num2 != num1)
            {
                i = (size_t) (cpu.cmd_buffer[i + 1] - 1);
            }
            else i++;

            break;

        case CALL:
            stack_push(&cpu.stk, (elem) (i + 1));
            i = (size_t) (cpu.cmd_buffer[i + 1] - 1);

            break;

        case RET:
            stack_pop(&cpu.stk, &num1);
            i = (size_t) num1;
            break;

        case IN:
            scanf("%lf", &num);
            stack_push(&cpu.stk, (elem) (num * ACCURACY));
            break;

        case PUSH_REG:
            switch (cpu.cmd_buffer[++i])
            {
                case AX:
                    stack_push(&cpu.stk, cpu.ax);
                    break;
                case BX:
                    stack_push(&cpu.stk, cpu.bx);
                    break;
                case CX:
                    stack_push(&cpu.stk, cpu.cx);
                    break;
                case DX:
                    stack_push(&cpu.stk, cpu.dx);
                    break;
            }
            break;

        case POP_REG:
            switch (cpu.cmd_buffer[++i])
            {
                case AX:
                    stack_pop(&cpu.stk, &cpu.ax);
                    break;
                case BX:
                    stack_pop(&cpu.stk, &cpu.bx);
                    break;
                case CX:
                    stack_pop(&cpu.stk, &cpu.cx);
                    break;
                case DX:
                    stack_pop(&cpu.stk, &cpu.dx);
                    break;
            }
            break;

        case SQRT:
            stack_pop(&cpu.stk, &num1);

            if (num1 < 0)
            {
                printf("Runtime error: try to sqrt number below zero\n");
                cpu_dtor(&cpu);

                return 2;
            }
            stack_push(&cpu.stk, (elem) (sqrt(num1) * sqrt(ACCURACY)));

            break;

        case NOROOTS:
            printf("no roots\n");
            break;

        case ALLNUM:
            printf("all numbers are roots\n");
            break;

        case HLT:
            cpu_dtor(&cpu);
            return 0;

        default:
            printf("Error: undefind command: %d\n", cpu.cmd_buffer[i]);
            cpu_dtor(&cpu);
            return 1;
        }
    }

    cpu_dtor(&cpu);
    return 0;
}

size_t cpu_ctor(s_cpu * cpu, FILE * stream)
{
    assert(cpu != NULL);
    assert(stream != NULL);

    stack_ctor(&cpu->stk, 5);

    fseek(stream, 0L, SEEK_END);
    size_t filesize = (size_t) ftell(stream);
    rewind(stream);

    size_t n_cmd = filesize / sizeof(elem);

    cpu->cmd_buffer = (elem *) calloc(n_cmd, sizeof(elem));
    fread(cpu->cmd_buffer, sizeof(elem), n_cmd, stream);

    return n_cmd;
}

void cpu_dtor(s_cpu * cpu)
{
    stack_dtor(&cpu->stk);
    free(cpu->cmd_buffer);
    //free(cpu->cpu_ram);

    cpu->cmd_buffer = NULL;
    cpu->cpu_ram = NULL;
    cpu->ax = 0;
    cpu->bx = 0;
    cpu->cx = 0;
    cpu->dx = 0;
}
