#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>

typedef enum
{
    START,
    IDENTIFIER_OR_KEYWORD,
    KEYWORD_CHECK,
    DIV_OR_COMMENT,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    MLC_END_CHECK,
    END_COMMENT
} FSMState;

void run_fsm(const unsigned char *input, FILE *out);
int compress_and_save(char* input_filepath, char* input_filename, const char* output_directory);
int compress_and_save_multiple(const char* input_directory_path, const char* output_directory_path);

#endif