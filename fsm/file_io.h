#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

int filename_has_ending(char *filename, const char *ending);
int is_code_file(char *filename);

char* build_filepath(const char *directory, char* filename);

char* get_original_filename_from_bin(char* bin_filename);
char* get_original_filename_from_c_or_h(char* bin_filename);

char* convert_bin_to_c_or_h(char* filename);
char* convert_bin_to_c(char* filename);
char* convert_bin_to_h(char* filename);

char* convert_c_or_h_to_bin(char* filename);
char* convert_c_to_bin(char* filename);
char* convert_h_to_bin(char* filename);

int get_file_length(const char *filepath);
int read_input_file(unsigned char *buffer, int file_len, const char *filepath);
void write_raw_byte(FILE *out, unsigned char ch);

typedef struct
{
    unsigned char *data;
    int data_bits;
    char *filename;
} compressed_file;

compressed_file *load_compressed_file(const char *filepath);
void free_compressed_file(compressed_file *cf);
compressed_file **load_multiple_compressed_files(const char *input_directory, int *file_count);
void free_multiple_compressed_files(compressed_file **files, int file_count);
void save_compressed_file(unsigned char* output, size_t output_len, char* output_filepath);

#endif
