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

// /* Writes the ASCII value of the character*/
// void write_raw_byte(FILE *out, unsigned char ch)
// {
//     fputc(ch, out);
// }

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
                write_raw_byte(out, input[i-2]);
                write_raw_byte(out, input[i-1]);
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

int compress_and_save(char* input_filepath, char* input_filename, const char* output_directory){
    size_t input_filename_len = strlen(input_filename);
    char output_filepath[256] = "";
    // building output_filepath
    strcat(output_filepath, output_directory);
    strcat(output_filepath, "/");
    strcat(output_filepath, input_filename);
    size_t output_filepath_len = strlen(output_filepath);
    // replace .c with .bin
    strcpy(output_filepath + output_filepath_len - 2 , ".bin");

    int file_len = get_file_length(input_filepath);
    unsigned char *compressor_input_buffer = (unsigned char *)malloc(file_len * sizeof(unsigned char));
    printf("====starting compression===\n");
    printf("number of chars in input: %d\n", file_len);

    read_input_file(compressor_input_buffer, file_len, input_filepath);

    FILE *out = fopen(output_filepath, "wb");
    if (out == NULL)
    {
        perror("Error: Failed to open output file");
        return 0;
    }

    run_fsm(compressor_input_buffer, out);
    fclose(out);

    printf("Compressor FSM run complete. Output written to %s\n", output_filepath);
    printf("====end compression===\n\n");
    return 1;
}

int compress_and_save_multiple(const char* input_directory, const char* output_directory)
{
    int num_files_in_dir = get_num_files_in_directory(input_directory);
    printf("number of files in input directory: %d\n", num_files_in_dir);
    char** filenames = malloc(num_files_in_dir * sizeof(char*));
    for(int i = 0 ; i < num_files_in_dir ; i ++){
        // assuming 256 is max filename length
        filenames[i] = malloc(sizeof(char) * 256);
    }
    get_files_in_directory(input_directory, num_files_in_dir, filenames);
    for(int i = 0 ; i < num_files_in_dir ; i ++){
        if(!is_c_file(filenames[i])){
            printf("[WARN] skipping input file: %s as it is not a c file\n", filenames[i]);
            continue;
        }

        char full_input_path[256] = "";
        strcat(full_input_path, input_directory);
        strcat(full_input_path, "/");
        strcat(full_input_path, filenames[i]);
        printf("compressing file at input path: %s\n", full_input_path);
        if(!compress_and_save(full_input_path, filenames[i], output_directory)){
            printf("Error: failed to compress file: %s", full_input_path);
        }
    }
    for(int i = 0 ; i < num_files_in_dir ; i ++){
        // assuming 256 is max filename length
        free(filenames[i]);
    }
    free(filenames);
    return 0;
}

