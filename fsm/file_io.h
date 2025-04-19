#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

int is_c_file(char* filename);
int is_bin_file(char* filename);
int get_file_length(const char *filepath);
int read_input_file(unsigned char *buffer, int file_len, const char *filepath);
void write_raw_byte(FILE *out, unsigned char ch);

typedef struct
{
    unsigned char *data;
    int data_bits;
    char *filename;
} compressed_file;

compressed_file **load_multiple_compressed_files(const char *input_directory);
compressed_file *load_compressed_file(const char *filepath);
void free_compressed_file(compressed_file *cf);

#endif