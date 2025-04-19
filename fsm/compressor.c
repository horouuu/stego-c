#include "compressor.h"
#include "keyword_map.h"
#include "file_io.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 1000
#define MAX_TOKEN 64

// /* Writes the ASCII value of the character*/
// void write_raw_byte(FILE *out, unsigned char ch)
// {
//     fputc(ch, out);
// }

void run_fsm(const char *input, FILE *out)
{
    FSMState state = START;
    int curr_char;
    char token[MAX_TOKEN];
    int token_len = 0;
    size_t i = 0;
    size_t token_i;
    unsigned char byte;

    while ((curr_char = input[i++]) != '\0')
    {
        switch (state)
        {
        case START:
            if (isalpha(curr_char))
            {
                token[token_len++] = curr_char;
                state = IDENTIFIER_OR_KEYWORD;
            }
            else if (curr_char == '/')
            {
                state = DIV_OR_COMMENT;
            }
            else if ((unsigned char)curr_char <= 127)
            {
                // write out any characters with ASCII code <= 127
                write_raw_byte(out, curr_char);
            }
            else
            {
                if (isprint(curr_char))
                {
                    printf("[WARN] Unexpected character skipped: '%c'\n", curr_char);
                }
                else
                {
                    printf("[WARN] Unexpected non-printable character skipped\n");
                    printf("ascii: %d\n", curr_char);
                    printf("char: %c\n", curr_char);
                }
            }
            break;
        case IDENTIFIER_OR_KEYWORD:
            if (isalpha((unsigned char)curr_char))
            {
                if (token_len < MAX_TOKEN - 1)
                    token[token_len++] = curr_char;
            }
            else
            {
                token[token_len] = '\0';
                i--; // reprocess later
                state = KEYWORD_CHECK;
            }
            break;
        case DIV_OR_COMMENT:
            if (curr_char == '/')
            {
                state = SINGLE_LINE_COMMENT;
            }
            else if (curr_char == '*')
            {
                state = MULTI_LINE_COMMENT;
            }
            else
            {
                state = START;
            }
            break;
        case SINGLE_LINE_COMMENT:
            if (curr_char == '\n')
            {
                write_raw_byte(out, curr_char);
                state = END_COMMENT;
            }
            break;
        case MULTI_LINE_COMMENT:
            if (curr_char == '*')
            {
                state = MLC_END_CHECK;
            }
            break;
        case MLC_END_CHECK:
            if (curr_char == '*')
            {
                state = MLC_END_CHECK;
            }
            else if (curr_char == '/')
            {
                state = END_COMMENT;
            }
            else
            {
                state = MULTI_LINE_COMMENT;
            }
            break;
        case END_COMMENT:
            // isalpha takes precedence
            if (isalpha(curr_char))
            {
                if (token_len < MAX_TOKEN - 1)
                    token[token_len++] = curr_char;

                state = IDENTIFIER_OR_KEYWORD;
            }
            else if (curr_char == '/')
            {
                state = DIV_OR_COMMENT;
            }
            else if ((unsigned char)curr_char <= 127)
            {
                // write out any characters with ASCII code <= 127
                write_raw_byte(out, curr_char);
                state = START;
            }
            else
            {
                state = START;
            }
            break;
        case KEYWORD_CHECK:
            if (is_keyword(token, &byte))
            {
                write_raw_byte(out, byte);
            }
            else
            {
                for (token_i = 0; token_i < token_len; token_i++)
                {
                    write_raw_byte(out, token[token_i]);
                }
            }
            token_len = 0;
            i--; // reprocess later
            state = START;
            break;
        }
    }
}

// int main()
// {
//     int file_len = get_file_length("input.txt");
//     char *input_buffer = (char *)malloc(file_len * sizeof(char));

//     read_input_file(input_buffer, file_len, "input.txt");
//     printf("buffer:\n %s\n", input_buffer);

//     const char *input = "int hello;";

//     FILE *out = fopen("output.bin", "wb");
//     if (out == NULL)
//     {
//         perror("Failed to open output file");
//         return 1;
//     }

//     run_fsm(input_buffer, out);
//     fclose(out);

//     printf("FSM run complete. Output written to output.bin\n");
//     return 0;
// }