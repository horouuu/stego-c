#include "compressor.h"
#include "keyword_map.h"
#include "file_io.h"
#include "directory_parser.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_LENGTH 1000
#define MAX_TOKEN 64

void run_fsm(const unsigned char *input, FILE *out)
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
                /* write out any characters with ASCII code <= 127 */
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
                i--; /* reprocess later */
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
                /* write out / and current char */
                write_raw_byte(out, input[i - 2]);
                if ((unsigned char)curr_char <= 127)
                    write_raw_byte(out, curr_char);
                state = START;
            }
            break;
        case SINGLE_LINE_COMMENT:
            if (curr_char == '\r' && input[i] == '\n')
            {
                write_raw_byte(out, curr_char);
            }
            else if (curr_char == '\n')
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
            /* isalpha takes precedence */
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
            else
            {
                /* write out any characters with ASCII code <= 127 */
                if ((unsigned char)curr_char <= 127)
                    write_raw_byte(out, curr_char);
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
            i--; /* reprocess later */
            state = START;
            break;
        }
    }
}

int compress_and_save(char *input_filepath, char *input_filename, const char *output_directory)
{
    char *output_filename = NULL;
    char *output_filepath = NULL;
    int file_len;
    unsigned char *compressor_input_buffer = NULL;
    FILE *out = NULL;

    output_filename = convert_c_or_h_to_bin(input_filename);
    if (output_filename == NULL)
    {
        perror("Error: Incorrect input filename format, filename must have format filename.h or filename.c, ending compression");
        return 0;
    }

    output_filepath = build_filepath(output_directory, output_filename);
    printf("output_filepath %s\n", output_filepath);

    file_len = get_file_length(input_filepath);
    compressor_input_buffer = (unsigned char *)malloc((file_len + 1) * sizeof(unsigned char));

    printf("====starting compression===\n");
    printf("number of chars in input: %d\n", file_len);

    read_input_file(compressor_input_buffer, file_len, input_filepath);

    out = fopen(output_filepath, "wb");
    if (out == NULL)
    {
        perror("Error: Failed to open output file");
        free(compressor_input_buffer);
        return 0;
    }

    printf("Running FSM...\n");
    run_fsm(compressor_input_buffer, out);

    printf("Compressor FSM run complete. Output written to %s\n", output_filepath);
    printf("====end compression===\n\n");

    fclose(out);
    free(compressor_input_buffer);
    free(output_filepath);
    free(output_filename);

    return 1;
}

int compress_and_save_multiple(const char *input_directory, const char *output_directory)
{
    int num_files_in_dir;
    char **filenames = NULL;
    char *input_filepath = NULL;
    int i, j, k;

    num_files_in_dir = get_num_files_in_directory(input_directory);
    printf("number of files in input directory: %d\n", num_files_in_dir);
    filenames = malloc(num_files_in_dir * sizeof(char *));

    for (i = 0; i < num_files_in_dir; i++)
    {
        /* assuming 256 is max filename length */
        filenames[i] = malloc(sizeof(char) * 256);
    }

    get_files_in_directory(input_directory, num_files_in_dir, filenames);

    for (j = 0; j < num_files_in_dir; j++)
    {
        if (!is_code_file(filenames[j]))
        {
            printf("[WARN] skipping input file: %s as it is not a c file\n", filenames[j]);
            continue;
        }

        input_filepath = build_filepath(input_directory, filenames[j]);

        printf("compressing file at input path: %s\n", input_filepath);
        if (!compress_and_save(input_filepath, filenames[j], output_directory))
        {
            printf("Error: failed to compress file: %s", input_filepath);
        }
        free(input_filepath);
    }

    for (k = 0; k < num_files_in_dir; k++)
    {
        /* assuming 256 is max filename length */
        free(filenames[k]);
    }

    free(filenames);
    return 0;
}
