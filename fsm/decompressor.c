#include "keyword_map.h"
#include "decompressor.h"
#include "file_io.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN 64

void run_decompressor(const unsigned char *input, FILE *out){

    int curr_char;
    size_t i = 0;
    size_t token_i;
    char* output_keyword = malloc(sizeof(char) * MAX_TOKEN);
    while ((curr_char = input[i++]) != '\0'){
        // printf("curr_char: %c\n",curr_char);
        if(is_compressed_keyword(curr_char, output_keyword)){
            for(int j = 0 ; j < strlen(output_keyword) ; j ++){
                write_raw_byte(out, output_keyword[j]);
            }
        } else {
            write_raw_byte(out, curr_char);
        }
    }
}

int decompress_and_save(char* input_filepath, char* output_filepath){

    printf("====starting decompression===\n");

    // decompressor
    int decompressor_input_file_len = get_file_length(input_filepath);
    unsigned char *decompressor_input_buffer = (unsigned char *)malloc(decompressor_input_file_len * sizeof(unsigned char));

    read_input_file(decompressor_input_buffer, decompressor_input_file_len, input_filepath);

    FILE *decompressor_out = fopen(output_filepath, "wb");
    if (decompressor_out == NULL)
    {
        perror("Error: Failed to open output file");
        return 0;
    }

    run_decompressor(decompressor_input_buffer, decompressor_out);
    fclose(decompressor_out);

    printf("Decompressor run complete. Output written to decompressed_input.txt\n");
    printf("====end decompression===\n");
    return 1;
}