#ifndef FILE_READER_H
#define FILE_READER_H
#include <stdio.h>
int get_file_length(const char *filepath);
int read_input_file(unsigned char* buffer, int file_len, char* filepath);
void write_raw_byte(FILE *out, unsigned char ch);
#endif