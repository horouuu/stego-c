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
    size_t i = 0, j = 0;
    char* output_keyword = malloc(sizeof(char) * MAX_TOKEN);
    for(i = 0 ; i < file_len ; i ++){
        curr_char = input[i];
        if(is_compressed_keyword(curr_char, output_keyword)){
            for(j = 0 ; j < strlen(output_keyword) ; j ++){
                write_raw_byte(out, output_keyword[j]);
            }
        } else {
            write_raw_byte(out, curr_char);
        }
    }
}

int decompress_and_save(char* input_filepath, char* input_filename, const char* output_directory){


    char* output_filename = convert_bin_to_c_or_h(input_filename);
    char* output_filepath = NULL;
    int decompressor_input_file_len;
    unsigned char *decompressor_input_buffer;
    FILE *decompressor_out;

    printf("====starting decompression===\n");

    if(output_filename == NULL){        
        perror("Error: Incorrect input filename format, filename must have format filename_h.bin or filename_c.bin, ending decompression");
        return 0;
    }
    output_filepath = build_filepath(output_directory, output_filename);
    /* decompressor */
    decompressor_input_file_len = get_file_length(input_filepath);
    decompressor_input_buffer = (unsigned char *)malloc((decompressor_input_file_len + 1)* sizeof(unsigned char));

    read_input_file(decompressor_input_buffer, decompressor_input_file_len, input_filepath);

    decompressor_out = fopen(output_filepath, "wb");
    if (decompressor_out == NULL)
    {
        perror("Error: Failed to open output file, ending decompression");
        return 0;
    }

    run_decompressor(decompressor_input_buffer, decompressor_input_file_len, decompressor_out);

    printf("Decompressor run complete. Output written to %s\n", output_filepath);
    printf("====end decompression===\n\n");

    fclose(decompressor_out);
    free(decompressor_input_buffer);
    free(output_filepath);
    free(output_filename);

    return 1;
}

int decompress_and_save_multiple(const char* input_directory, const char* output_directory)
{

    int num_files_in_dir = get_num_files_in_directory(input_directory);
    char** filenames = malloc(num_files_in_dir * sizeof(char*));
    int i;
    char* input_filepath;
    printf("number of files in input directory: %d\n", num_files_in_dir);

    for(i = 0 ; i < num_files_in_dir ; i ++){
        /* assuming 256 is max filename length */
        filenames[i] = malloc(sizeof(char) * 256);
    }
    get_files_in_directory(input_directory, num_files_in_dir, filenames);
    for(i = 0 ; i < num_files_in_dir ; i ++){
        if(!filename_has_ending(filenames[i], ".bin")){
            printf("[WARN] decompressor skipping input file: %s as it is not a bin file\n", filenames[i]);
            continue;
        }

        input_filepath = build_filepath(input_directory, filenames[i]);
        printf("compressing file at input path: %s\n", input_filepath);
        if(!decompress_and_save(input_filepath, filenames[i], output_directory)){
            printf("Error: failed to decompress file: %s", input_filepath);
        }
        free(input_filepath);
    }
    for(i = 0 ; i < num_files_in_dir ; i ++){
        /* assuming 256 is max filename length */
        free(filenames[i]);
    }
    free(filenames);
    return 0;
}
