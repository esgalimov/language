#include "asm.h"

void run_comp(FILE * stream)
{
    asm_t asem = {};
    asm_ctor(&asem, stream);

    size_t i = 0, i_code = 0;

    while (i < asem.commands.str_cnt)
    {
        char cmd[MAX_CMD_LEN] = "";
        asem.len_cmd = 0;
        asem.len_cmd_gap = 0;
        sscanf(asem.commands.strings[i], "%s%n", cmd, &asem.len_cmd);

        if (is_without_text(cmd))
        {
            i++;
            continue;
        }

        while (asem.len_cmd > 0)
        {
            asem.toks[i_code].name = (char *) calloc(MAX_CMD_LEN, sizeof(char));

            if (cmd[0] == ';') break;

            else if (strcmp(cmd, "push") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = PUSH;

            }
            else if (strcmp(cmd, "add") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = ADD;
            }
            else if (strcmp(cmd, "sub") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = SUB;
            }
            else if (strcmp(cmd, "div") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = DIV;
            }
            else if (strcmp(cmd, "mul") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = MUL;
            }
            else if (strcmp(cmd, "pop") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = POP;
            }
            else if (strcmp(cmd, "out") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = OUT;
            }
            else if (strcmp(cmd, "hlt") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = HLT;
            }
            else if (strcmp(cmd, "jmp") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JMP;
            }
            else if (strcmp(cmd, "jb") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JB;
            }
            else if (strcmp(cmd, "jbe") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JBE;
            }
            else if (strcmp(cmd, "ja") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JA;
            }
            else if (strcmp(cmd, "jae") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JAE;
            }
            else if (strcmp(cmd, "je") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JE;
            }
            else if (strcmp(cmd, "jne") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JNE;
            }
            else if (strcmp(cmd, "call") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = CALL;
            }
            else if (strcmp(cmd, "ret") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = RET;
            }
            else if (strcmp(cmd, "in") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = IN;
            }
            else if (str_of_digits(cmd))
            {
                double value = 0;
                sscanf(cmd, "%lf", &value);

                asem.toks[i_code].type = NUM;
                asem.toks[i_code].value = (int) (value * ACCURACY);
            }
            else if (is_label(cmd))
            {
                asem.toks[i_code].type = LABEL;
                asem.label_toks[asem.labels_cnt] = &asem.toks[i_code];

                int i_label = find_label(&asem, cmd);

                if (i_label == -1)
                {
                    asem.toks[i_code].value = asem.labels_cnt++;
                }
                else
                {
                    asem.toks[i_code].value = i_label;
                }
            }
            else if (cmd[0] == '[')
            {
                int ram_type = is_ram(cmd);

                switch (ram_type)
                {
                    case TYPE_NOT_RAM: asem.toks[i_code].type = UNDEFIND; break;

                    case TYPE_NUM_RAM: case TYPE_NUM_REG_RAM:
                    {
                        int num_ram = 0, len_num = 0;
                        sscanf(cmd + 1, "%d%n", &num_ram, &len_num);

                        asem.toks[i_code].type = NUM_RAM;
                        asem.toks[i_code].value = num_ram;

                        if (ram_type == TYPE_NUM_RAM) break;

                        strcpy(asem.toks[i_code].name, cmd);
                        asem.toks[i_code].line = (int) (i + 1);
                        i_code++;

                        asem.toks[i_code].name = (char *) calloc(MAX_CMD_LEN, sizeof(char));
                        asem.toks[i_code].type = REG_RAM;

                        switch (cmd[len_num + 2])
                        {
                            case 'a': asem.toks[i_code].value = AX; break;
                            case 'b': asem.toks[i_code].value = BX; break;
                            case 'c': asem.toks[i_code].value = CX; break;
                            case 'd': asem.toks[i_code].value = DX; break;
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "ax") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = AX;
            }
            else if (strcmp(cmd, "bx") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = BX;
            }
            else if (strcmp(cmd, "cx") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = CX;
            }
            else if (strcmp(cmd, "dx") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = DX;
            }
            else if (strcmp(cmd, "sqrt") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = SQRT;
            }
            else if (strcmp(cmd, "noroots") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = NOROOTS;
            }
            else if (strcmp(cmd, "allnum") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = ALLNUM;
            }
            else
            {
                asem.toks[i_code].type = UNDEFIND;
            }

            strcpy(asem.toks[i_code].name, cmd);
            asem.toks[i_code].line = (int) (i + 1);
            i_code++;

            asem.len_cmd_gap += asem.len_cmd;
            asem.len_cmd = 0;

            sscanf(asem.commands.strings[i] + asem.len_cmd_gap, "%s%n", cmd, &asem.len_cmd);

            check_toks_for_size(&asem, i_code);
        }
        i++;
    }

    labels_init(&asem, i_code);

    if (check_code(&asem, i_code))
    {
        make_label_jmp_push_reg(&asem, i_code);

        write_code_to_file(asem.toks, i_code);
        printf("Compiled OK\n");
    }

    asm_dtor(&asem);
}

void write_code_to_file(token_t * toks, size_t n_cmd)
{
    assert(toks != NULL);

    FILE * fp = fopen("./system_files/prog.code", "w");
    FILE * fp_bin = fopen("./system_files/prog.bin", "wb");

    if (fp == NULL || fp_bin == NULL)
    {
        printf("Can't open file (asm)\n");
        abort();
    }

    elem_t * code = (elem_t *) calloc(n_cmd, sizeof(elem_t));
    size_t label_found = 0;

    for (size_t i = 0; i < n_cmd; i++)
    {
        if (toks[i].type == LABEL)
        {
            label_found++;
        }
        else
        {
            code[i - label_found] = toks[i].value;
            fprintf(fp, "%d ", toks[i].value);
        }

    }

    fwrite(code, sizeof(elem_t), n_cmd, fp_bin);

    fclose(fp);
    fclose(fp_bin);

    free(code);
}

int check_code(asm_t * asem, size_t n_cmd)
{
    assert(asem != NULL);

    int is_hlt = 0, is_ok = 1;

    for (size_t i = 0; i < n_cmd; i++)
    {
        if (IS_UNDEFIND(i))
            ERROR_MESSAGE("not found at line");

        else if (IS_CMD1(i) && IS_PUSH(i))
        {
            if ((i + 1 < n_cmd && (!IS_NUM(i+1) && !IS_REG(i+1) && !IS_NUM_RAM(i+1))) || (i + 1 == n_cmd))
                ERROR_MESSAGE("has not given an argument, but it must have 1 argument");

            else if (i+2 < n_cmd)
            {
                if ((IS_NUM(i+1) || IS_REG(i+1)) && (IS_NUM(i+2) || IS_REG(i+2)))
                    ERROR_MESSAGE("has given more than 1 register or number argument, but it must have 1 such argument");
                else if (IS_NUM_RAM(i+1) && (IS_NUM(i+2) || IS_REG(i+2)))
                    ERROR_MESSAGE("has given NUM_RAM and register or number after it, but it must have 1 such argument or have REG_RAM after");
                else if (i+3 < n_cmd && (IS_NUM_RAM(i+1) && IS_REG_RAM(i+2) && (IS_NUM(i+3) || IS_REG(i+3) || IS_REG_RAM(i+3) || IS_NUM_RAM(i+3))))
                    ERROR_MESSAGE("has given sth after NUM_RAM + REG_RAM");
                else if (IS_REG_RAM(i+1))
                    ERROR_MESSAGE("has given only REG_RAM");
            }
        }

        else if (IS_CMD1(i) && check_for_jump(asem->toks[i].value))
        {
            if ((i + 1 < n_cmd && !IS_LABEL(i+1)) || (i + 1) == n_cmd)
                ERROR_MESSAGE("has not given an argument, but it must have 1 argument");

            else if (asem->labels[asem->toks[i + 1].value].value == -1)
                    ERROR_MESSAGE("label doesn't exist");
        }

        else if (IS_LABEL(i))
        {
            if (asem->labels[asem->toks[i].value].cnt > 1)
                ERROR_MESSAGE("there are more than one label to jump");
        }

        else if (IS_CMD0(i))
        {
            if (i + 1 < n_cmd && IS_NUM(i+1))
                ERROR_MESSAGE("must not have arguments");
            else if (i + 2 < n_cmd && IS_POP(i))
            {
                if (IS_REG(i+1) && IS_REG(i+2))
                    ERROR_MESSAGE("can not have more than one register as argument");
                else if (IS_REG(i+1) && (IS_REG_RAM(i+2) || IS_NUM_RAM(i+2)))
                    ERROR_MESSAGE("can not have register and REG_RAM or NUM_RAM");
                else if (IS_REG_RAM(i+1))
                    ERROR_MESSAGE("can not have only REG_RAM");
                else if (IS_NUM_RAM(i+1) && IS_NUM_RAM(i+2))
                    ERROR_MESSAGE("can not have NUM_RAM after NUM_RAM");
                else if ((IS_REG(i+1) || IS_REG_RAM(i+1) || IS_NUM_RAM(i+1)) && IS_NUM(i+2))
                    ERROR_MESSAGE("can not have num in args");
                else if (i + 3 < n_cmd && IS_NUM_RAM(i+1) && IS_REG_RAM(i+2) && (IS_NUM_RAM(i+3) || IS_REG_RAM(i+3) || IS_REG(i+3)))
                    ERROR_MESSAGE("can not have sth after NUM_RAM + REG_RAM");
            }

            else if (IS_HLT(i)) is_hlt = 1;
        }
    }

    if (is_hlt == 0)
    {
        printf("Error: there is no hlt command in programm\n");
        is_ok = 0;
    }

    return is_ok;
}

void print_error_message(int* is_ok, size_t i, asm_t* asem, const char* message)
{
    printf("Error: invalid syntax at line %d: %s %s \n", asem->toks[i].line + 1, asem->toks[i].name, message);
    *is_ok = 0;
}


int str_of_digits(const char* cmd)
{
    size_t len_cmd = strlen(cmd);
    size_t i = 0;
    int is_dotted = 0;

    if (cmd[0] == '-')
        i = 1;

    for (; i < len_cmd; i++)
    {
        if (!is_dotted && cmd[i] == '.')
        {
            is_dotted = 1;
        }
        else if (isdigit(cmd[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

int is_label(const char * cmd)
{
    if (cmd[0] != ':')
    {
        return 0;
    }
    else
    {
        size_t len_cmd = strlen(cmd);

        for (size_t i = 1; i < len_cmd; i++)
        {
            char ch = cmd[i];
            if (isdigit(ch) == 0 && isalpha(ch) == 0 && ch != '_') return 0;
        }
        return 1;
    }
}

int is_ram(const char* cmd)
{
    if (cmd[0] != '[') return TYPE_NOT_RAM;

    size_t len_cmd = strlen(cmd),
           i = 1;

    if (cmd[1] == '-') i++;

    for (; i < len_cmd; i++)
    {
        if (isdigit(cmd[i])) continue;

        else if (cmd[i] == '+') break;

        else if (cmd[i] == ']') return TYPE_NUM_RAM;

        else return TYPE_NOT_RAM;
    }

    if (cmd[++i] == ']') return TYPE_NUM_RAM;                                             // like [35]

    if ('a' <= cmd[i] && cmd[i] <= 'd' && !strcmp(cmd + i + 1, "x]")) return TYPE_NUM_REG_RAM; // like [27+ax]

    return TYPE_NOT_RAM;
}

int find_label(asm_t* asem, const char* name)
{
    for (int i = 0; i < asem->labels_cnt; i++)
    {
        if (!strcmp(asem->label_toks[i]->name, name))
            return asem->label_toks[i]->value;
    }
    return -1;
}

void check_toks_for_size(asm_t * asem, size_t i_code)
{
    if (asem->toks_cnt <= i_code)
    {
        asem->toks_cnt *= 2;
        asem->toks = (token_t*) realloc(asem->toks, asem->toks_cnt * sizeof(elem_t));
    }
}

void asm_ctor(asm_t* asem, FILE* stream)
{
    asem->commands = {};

    text_ctor(&asem->commands, stream);

    asem->toks = (token_t*) calloc(asem->commands.str_cnt * 2, sizeof(token_t));

    asem->labels = (label_t*) calloc(N_LABELS, sizeof(label_t));

    asem->label_toks = (token_t**) calloc(N_LABELS, sizeof(token_t*)); //?????

    for (int i = 0; i < N_LABELS; i++)
        asem->labels[i].value = -1;

    asem->toks_cnt = asem->commands.str_cnt * 2;

    asem->len_cmd = 0;
    asem->len_cmd_gap = 0;
}

void asm_dtor(asm_t * asem)
{
    assert(asem != NULL);

    text_dtor(&asem->commands);

    asem->len_cmd = 0;
    asem->len_cmd_gap = 0;
    asem->toks_cnt = 0;

    free(asem->labels);
    free(asem->toks);
    asem->labels = NULL;
    asem->toks = NULL;

}

void labels_init(asm_t * asem, size_t n_toks)
{
    size_t label_found = 0;

    if (asem->toks[0].type == LABEL)
    {
        asem->labels[asem->toks[0].value].value = 0;
        asem->labels[asem->toks[0].value].cnt++;
        label_found++;
    }

    size_t i = 0;

    for (i = 1; i < n_toks; i++)
    {
        if (asem->toks[i].type == LABEL)
        {
            if (asem->toks[i - 1].type == CMD1 && check_for_jump(asem->toks[i - 1].value))
                continue;

            asem->labels[asem->toks[i].value].value = (int) (i - label_found);
            asem->labels[asem->toks[i].value].cnt++;
            label_found++;
        }
    }
}

void make_label_jmp_push_reg(asm_t * asem, size_t n_toks)
{
    for (size_t i = 0; i < n_toks; i++)
    {
        if (IS_CMD1(i) && check_for_jump(asem->toks[i].value))
        {
            asem->toks[i + 1].type = LABEL_JMP;
            asem->toks[i + 1].value = asem->labels[asem->toks[i + 1].value].value;
        }

        if (IS_CMD1(i) && IS_PUSH(i))
        {
            if (IS_REG(i+1))
            {
                asem->toks[i].value = PUSH_REG;
            }
            else if (IS_REG_RAM(i+1) || IS_NUM_RAM(i+1))
            {
                asem->toks[i].value = PUSH_RAM;
            }
        }

        if (IS_CMD0(i) && IS_POP(i))
        {
            if (IS_REG(i+1))
            {
                asem->toks[i].value = POP_REG;
                asem->toks[i].type = CMD1;
            }
            else if (IS_REG_RAM(i+1) || IS_NUM_RAM(i+1))
            {
                asem->toks[i].value = POP_RAM;
                asem->toks[i].type = CMD1;
            }
        }
    }
}

int check_for_jump(int value)
{
    return (value >= JMP && value <= CALL);
}
