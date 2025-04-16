#include <stdio.h>
#include "fsm.h"

int is_keyword(const char *token, unsigned char *output_byte);

#define MAX_TOKEN 64

// Writes the ASCII value of the character
void write_raw_byte(FILE *out, unsigned char ch)
{
    fputc(ch, out);
}

void run_fsm(const char *input, FILE *out)
{
    enum FSMState state = START;
    int curr_char;
    char token[MAX_TOKEN];
    int token_len = 0;
    size_t i = 0;
    size_t token_i;

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
            else
            {
                write_raw_byte(out, curr_char);
            }
            break;

        case IDENTIFIER_OR_KEYWORD:
            if (isalpha(curr_char))
            {
                if (token_len < MAX_TOKEN - 1)
                    token[token_len++] = curr_char;
            }
            else
            {
                token[token_len] = '\0';
                unsigned char byte;
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
                state = START;
                i--;
            }
            break;
        }
    }
}