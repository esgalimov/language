#include "asm.h"
#include "text_funcs.h"

char** import_text(text_t* book, FILE* stream)
{
    ASSERT(stream != NULL);
    ASSERT(book != NULL);

    fseek(stream, 0L, SEEK_END);
    book->buffsize = (size_t) ftell(stream);
    rewind(stream);

    book->buffer = (char*) calloc(book->buffsize + 1, sizeof(char));
    fread(book->buffer, sizeof(char), book->buffsize, stream);
    *(book->buffer + book->buffsize) = '\0';

    book->str_cnt = count_symbol('\n', book->buffer, book->buffsize);

    return get_ptrs(book->buffer, book->str_cnt, book->buffsize);
}


size_t count_symbol(char ch, char* string, size_t strsize)
{
    ASSERT(string != NULL);

    size_t str_cnt = 0;

    for (size_t i = 0; i < strsize; i++)
    {
        if (string[i] == ch)
            str_cnt++;
    }

    return str_cnt;
}


char** get_ptrs(char* buffer, size_t str_cnt, size_t buffsize)
{
    ASSERT(buffer != NULL);

    char** strptr = (char**) calloc((size_t) (str_cnt + 1), sizeof(char*));

    if (strptr == NULL) return NULL;

    strptr[0] = &buffer[0];
    size_t str_i = 1;
    for (size_t ch_i = 1; ch_i < buffsize; ch_i++)
        if (buffer[ch_i] == '\n')
        {
            if (str_i < str_cnt)
            {
                buffer[ch_i] = '\0';
                strptr[str_i] = &buffer[ch_i + 1];
                str_i++;
            }
        }
    strptr[str_i] = NULL;

    return strptr;
}

int is_without_text(const char* str)
{
    assert(str != NULL);
    int flag = 1;
    int len = (int) strlen(str);
    for (int i = 0; i < len; i++)
        if (!isblank(str[i]))
        {
            flag = 0;
            break;
        }
    return flag;
}

void text_ctor(text_t* book, FILE* stream)
{
    ASSERT(book != NULL);
    ASSERT(stream != NULL);

    book->strings  = NULL;
    book->buffer   = NULL;
    book->str_cnt  = 0;
    book->buffsize = 0;

    book->strings = import_text(book, stream);
    fclose(stream);
}

void text_dtor(text_t* book)
{
    ASSERT(book != NULL);

    book->str_cnt = 0;
    book->buffsize = 0;

    free(book->buffer);
    free(book->strings);

    book->strings = NULL;
    book->buffer = NULL;
}
