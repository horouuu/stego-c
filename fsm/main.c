#include "file_io.h"
#include "decompressor.h"
#include "compressor.h"
#include "directory_parser.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(void)
{
    int file_count = 0;
    const char *compressor_input_directory = "../compressor_input";
    const char *compressor_output_directory = "../compressor_output";
    const char *decompressor_output_directory = "../decompressor_output";
    compressed_file **cfiles = NULL;
    int i, j;

    compress_and_save_multiple(compressor_input_directory, compressor_output_directory);
    decompress_and_save_multiple(compressor_output_directory, decompressor_output_directory);

    cfiles = load_multiple_compressed_files(compressor_output_directory, &file_count);
    if (cfiles)
    {
        for (i = 0; i < file_count; i++)
        {
            compressed_file *cf = cfiles[i];
            printf("Filename: %s\n", cf->filename);
            printf("File size: %d bits\n", cf->data_bits);
            printf("Bytes: ");
            for (j = 0; j < cf->data_bits / 8; j++)
            {
                printf("%02X ", cf->data[j]);
            }
            printf("\n");
        }

        free_multiple_compressed_files(cfiles, file_count);
    }

    return 0;
}
