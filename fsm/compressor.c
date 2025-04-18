#include <stdio.h>
#include <ctype.h>
#include "compressor.h"
#include "keyword_map.h"

#define MAX_TOKEN 64

// Writes the ASCII value of the character
void write_raw_byte(FILE *out, unsigned char ch)
{
    fputc(ch, out);
}

void run_fsm(const char *input, FILE *out)
{
    FSMState state = START;
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

int main()
{
    const char *input = "int hello;";

    FILE *out = fopen("output.bin", "wb");
    if (out == NULL)
    {
        perror("Failed to open output file");
        return 1;
    }

    run_fsm(input, out);
    fclose(out);

    printf("FSM run complete. Output written to output.bin\n");
    return 0;
}