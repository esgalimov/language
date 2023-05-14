#ifndef TEXT_FUNCS
#define TEXT_FUNCS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LOG_MODE

typedef struct
{
    char** strings;
    char*  buffer;
    size_t    str_cnt;
    size_t    buffsize;
} text_t;

//! @brief Function that import text from file and write it into the buffer
//! in allocated memory with calloc
//!
//! @param [out] book - pointer to struct with variables for text processing
//! @param [in] stream - pointer to the file with text
//!
//! @return Pointer to array with pointers to string
//! @note Function use get_ptrs to make array with pointers to strings
char** import_text(text_t* book, FILE* stream);

//! @brief Function for counting number of particular symbol
//!
//! @param [in] ch - symbol that is counting
//! @param [in] string - pointer to string where we want to count
//! @param [in] strsizes - size of string in bytes
//!
//! @return Number of symbol ch
size_t count_symbol(char ch, char* string, size_t strsizes);

//! @brief Function that swap "\n" to "\0" in buffer and make array of pointers to strings in buffer
//!
//! @param [in] buffer - pointer to buffer
//! @param [in] str_cnt - number of strings
//! @param [in] buffsize - size of file in bytes
//!
//! @return pointer to array with pointers to stings
char ** get_ptrs(char* buffer, size_t str_cnt, size_t buffsize);

//! @brief Constructor for Text
//!
//! @param [out] book - pointer to struct with variables for text processing
//! @param [in] stream - pointer to file with text
//!
//! @note Initialize struct fields, use import_text, close file stream
void text_ctor(text_t* book, FILE* strem);

//! @brief Destruct for Text
//! @param [out] book - pointer to struct with variables for text processing
void text_dtor(text_t* book);

//! @brief Function for checking if string consists of blank symbol only
//! @param [in] str - pointer to string for checking
//! @return 1 - if only blank symbols, else - 0

int is_without_text(const char* str);

#endif
