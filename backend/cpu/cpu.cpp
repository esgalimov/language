#include "cpu.h"

int run_cpu(FILE * stream)
{
    assert(stream != NULL);

    cpu_t cpu = {};
    size_t n_cmd = cpu_ctor(&cpu, stream);

    double num = NAN;
    elem_t num1 = 0, num2 = 0;

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
            if (num1 == 0)
            {
                printf("division by zero\n");
                return 1;
            }
            stack_push(&cpu.stk, (elem_t) (((double) num2 / (double) num1) * ACCURACY));
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
            printf("OUT: %lg\n", (double) num1 / ACCURACY);
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
            stack_push(&cpu.stk_ret, (elem_t) i + 1);
            i = (size_t) (cpu.cmd_buffer[i + 1] - 1);

            break;

        case RET:
            stack_pop(&cpu.stk_ret, &num1);
            i = (size_t) num1;
            break;

        case IN:
            scanf("%lf", &num);
            ASSERT(isfinite(num));
            stack_push(&cpu.stk, (elem_t) (num * ACCURACY));
            break;

        case PUSH_REG:
            stack_push(&cpu.stk, *get_reg_ptr(&cpu, ++i));
            break;

        case POP_REG:
            stack_pop(&cpu.stk, &num1);
            *get_reg_ptr(&cpu, ++i) = num1;
            break;

        case PUSH_RAM:
        {
            i++;
            printf("It is push_ram\n");

            if (AX <= cpu.cmd_buffer[i + 1] && cpu.cmd_buffer[i + 1] <= DX)
            {
                elem_t reg_val = *(get_reg_ptr(&cpu, i + 1)) / ACCURACY;



                if ((int) reg_val + cpu.cmd_buffer[i] > RAM_SIZE)
                {
                    printf("Out of RAM\n"); return 1;
                }
                stack_push(&cpu.stk, cpu.cpu_ram[(int) reg_val + cpu.cmd_buffer[i]]);
                printf("index in ram = %d\n", (int) reg_val + cpu.cmd_buffer[i]);
                i++;
            }
            else
            {
                stack_push(&cpu.stk, cpu.cpu_ram[cpu.cmd_buffer[i]]);
                printf("index in ram = %d\n", cpu.cmd_buffer[i]);
            }
            for (int x = 0; x < 20; x++)
                printf("%5d", cpu.cpu_ram[x]);
            printf("\n");

            break;
        }

        case POP_RAM:
        {
            i++;
            printf("It is pop_ram\n");

            if (AX <= cpu.cmd_buffer[i + 1] && cpu.cmd_buffer[i + 1] <= DX)
            {
                elem_t reg_val = *(get_reg_ptr(&cpu, i + 1)) / ACCURACY;

                if ((int) reg_val + cpu.cmd_buffer[i] > RAM_SIZE)
                {
                    printf("Out of RAM: want to get index = %d\n", (int) reg_val + cpu.cmd_buffer[i]);
                        return 1;
                }

                stack_pop(&cpu.stk, &cpu.cpu_ram[(int) reg_val + cpu.cmd_buffer[i]]);

                printf("index in ram = %d\n", (int) reg_val + cpu.cmd_buffer[i]);
                i++;
            }
            else
            {
                stack_pop(&cpu.stk, &cpu.cpu_ram[cpu.cmd_buffer[i]]);
                printf("index in ram = %d\n", cpu.cmd_buffer[i]);
            }

            for (int x = 0; x < 20; x++)
                printf("%5d", cpu.cpu_ram[x]);
            printf("\n");

            break;
        }

        case SQRT:
            stack_pop(&cpu.stk, &num1);

            if (num1 < 0)
            {
                printf("Runtime error: try to sqrt number below zero\n");
                cpu_dtor(&cpu);

                return 2;
            }
            stack_push(&cpu.stk, (elem_t) (sqrt(num1) * sqrt(ACCURACY)));

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

elem_t* get_reg_ptr(cpu_t* cpu, size_t i)
{
    switch (cpu->cmd_buffer[i])
    {
        case AX: return &cpu->ax;
        case BX: return &cpu->bx;
        case CX: return &cpu->cx;
        case DX: return &cpu->dx;
        default: return nullptr;
    }
}

size_t cpu_ctor(cpu_t* cpu, FILE* stream)
{
    ASSERT(cpu != NULL);
    ASSERT(stream != NULL);

    stack_ctor(&cpu->stk,     MIN_CAPACITY);
    stack_ctor(&cpu->stk_ret, MIN_CAPACITY);

    fseek(stream, 0L, SEEK_END);
    size_t filesize = (size_t) ftell(stream);
    rewind(stream);

    size_t n_cmd = filesize / sizeof(elem_t);

    cpu->cmd_buffer = (elem_t*) calloc(n_cmd, sizeof(elem_t));
    fread(cpu->cmd_buffer, sizeof(elem_t), n_cmd, stream);

    cpu->cpu_ram = (elem_t*) calloc(RAM_SIZE, sizeof(elem_t));

    return n_cmd;
}

void cpu_dtor(cpu_t * cpu)
{
    stack_dtor(&cpu->stk);
    stack_dtor(&cpu->stk_ret);

    free(cpu->cmd_buffer);
    free(cpu->cpu_ram);

    cpu->cmd_buffer = NULL;
    cpu->cpu_ram = NULL;
    cpu->ax = 0;
    cpu->bx = 0;
    cpu->cx = 0;
    cpu->dx = 0;
}
