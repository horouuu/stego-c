#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H
#include <stdio.h>

void run_decompressor(const unsigned char *input, FILE *out);
int decompress_and_save(char* input_filepath, char* output_filepath);

#endif
