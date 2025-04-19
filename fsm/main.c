#include "file_io.h"
#include "decompressor.h"
#include "compressor.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{

    // // compressor
    char *compressor_input_filename = "input.txt";
    char *compressor_output_filename = "output.bin";

    if(!compress_and_save(compressor_input_filename, compressor_output_filename)){
        perror("Error: compression failed");
        return 1;
    }
    compressed_file *cf = load_compressed_file(compressor_output_filename);
    if (cf)
    {
        printf("Filename: %s\n", cf->filename);
        printf("File size: %d bits\n", cf->data_bits);
        printf("Bytes: ");
        for (int i = 0; i < cf->data_bits / 8; i++)
        {
            printf("%02X ", cf->data[i]);
        }
        printf("\n");

        free_compressed_file(cf);
    }
    char *decompressor_output_filename = "decompressed_input.txt";

    if(!decompress_and_save(compressor_output_filename,decompressor_output_filename)){
        perror("Error: decompression failed");
        return 1;
    }
    return 0;
}