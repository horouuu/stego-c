#include "file_io.h"
#include "decompressor.h"
#include "compressor.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(){

    // // compressor
    char* compressor_input_filename = "input.txt";
    char* compressor_output_filename = "output.bin";
    int file_len = get_file_length(compressor_input_filename);
    char *compressor_input_buffer = (char *)malloc(file_len * sizeof(unsigned char));
    printf("====starting compression===\n");
    printf("number of chars in input: %d\n", file_len);

    read_input_file(compressor_input_buffer, file_len, compressor_input_filename);


    FILE *out = fopen(compressor_output_filename, "wb");
    if (out == NULL)
    {
        perror("Failed to open output file");
        return 1;
    }

    run_fsm(compressor_input_buffer, out);
    fclose(out);

    printf("Compressor FSM run complete. Output written to output.bin\n");
    printf("====end compression===\n\n");
    printf("====starting decompression===\n");

    // decompressor
    char* decompressor_output_filename = "decompressed_input.txt";
    int decompressor_input_file_len = get_file_length(compressor_output_filename);
    char *decompressor_input_buffer = (char *)malloc(decompressor_input_file_len * sizeof(char));

    read_input_file(decompressor_input_buffer, decompressor_input_file_len, compressor_output_filename);

    FILE *decompressor_out = fopen(decompressor_output_filename, "wb");
    if (decompressor_out == NULL)
    {
        perror("Failed to open output file");
        return 1;
    }

    run_decompressor(decompressor_input_buffer, decompressor_out);
    fclose(decompressor_out);

    printf("Decompressor run complete. Output written to decompressed_input.txt\n");
    printf("====end decompression===\n");

    return 0;
}