#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H
#include <stdio.h>

void run_decompressor(const unsigned char *input, int file_len, FILE *out);
int decompress_and_save(char* input_filepath, char* input_filename, const char* output_directory);
int decompress_and_save_multiple(const char* input_directory, const char* output_directory);
#endif
