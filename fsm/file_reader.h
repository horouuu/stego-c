#ifndef FILE_READER_H
#define FILE_READER_H
int get_file_length(const char *filepath);
int read_input_file(char* buffer, int file_len, char* filepath);
#endif