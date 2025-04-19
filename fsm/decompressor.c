#include "keyword_map.h"
#include "decompressor.h"
#include "file_io.h"
#include "directory_parser.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN 64

void run_decompressor(const unsigned char *input, int file_len, FILE *out){

    int curr_char;
    size_t i = 0;
    size_t token_i;
    char* output_keyword = malloc(sizeof(char) * MAX_TOKEN);
    for(int i = 0 ; i < file_len ; i ++){
        curr_char = input[i];
        if(is_compressed_keyword(curr_char, output_keyword)){
            for(int j = 0 ; j < strlen(output_keyword) ; j ++){
                write_raw_byte(out, output_keyword[j]);
            }
        } else {
            write_raw_byte(out, curr_char);
        }
    }
}

int decompress_and_save(char* input_filepath, char* input_filename, const char* output_directory){

    printf("====starting decompression===\n");
    size_t input_filename_len = strlen(input_filename);
    char output_filepath[256] = "";
    // building output_filepath
    strcat(output_filepath, output_directory);
    strcat(output_filepath, "/");
    strncat(output_filepath, input_filename, input_filename_len - 2);
    size_t output_filepath_len = strlen(output_filepath);
    // replace .bin with .c
    strcpy(output_filepath + output_filepath_len - 2 , ".c");
    // decompressor
    int decompressor_input_file_len = get_file_length(input_filepath);
    unsigned char *decompressor_input_buffer = (unsigned char *)malloc((decompressor_input_file_len + 1)* sizeof(unsigned char));

    read_input_file(decompressor_input_buffer, decompressor_input_file_len, input_filepath);

    FILE *decompressor_out = fopen(output_filepath, "wb");
    if (decompressor_out == NULL)
    {
        perror("Error: Failed to open output file");
        return 0;
    }

    run_decompressor(decompressor_input_buffer, decompressor_input_file_len, decompressor_out);
    fclose(decompressor_out);

    printf("Decompressor run complete. Output written to decompressed_input.txt\n");
    printf("====end decompression===\n");
    return 1;
}

int decompress_and_save_multiple(const char* input_directory, const char* output_directory)
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
        if(!is_bin_file(filenames[i])){
            printf("[WARN] decompressor skipping input file: %s as it is not a bin file\n", filenames[i]);
            continue;
        }

        char full_input_path[256] = "";
        strcat(full_input_path, input_directory);
        strcat(full_input_path, "/");
        strcat(full_input_path, filenames[i]);
        printf("compressing file at input path: %s\n", full_input_path);
        decompress_and_save(full_input_path, filenames[i], output_directory);
    }
    return 0;
}