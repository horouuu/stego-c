#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "directory_parser.h"
int is_c_file(char* filename){
    size_t len = strlen(filename);
    if (len < 2) 
        return 0; 
    return strncmp(filename + len - 2, ".c", 2) == 0;
}
int is_bin_file(char* filename){
    size_t len = strlen(filename);
    if (len < 4) 
        return 0; 
    return strncmp(filename + len - 4, ".bin", 4) == 0;
}
int get_file_length(const char *filepath)
{

    FILE *fp = NULL;
    fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        perror("Error: ");
        return -1;
    }
    int c;
    int file_len = 0;
    while ((c = fgetc(fp) != EOF))
    {
        file_len++;
    }
    return file_len;
}

int read_input_file(unsigned char *buffer, int file_len, const char *filepath)
{
    FILE *fp = NULL;
    fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        perror("Error: ");
        return -1;
    }
    fread(buffer, 1, file_len, fp);
    buffer[file_len] = '\0';
    fclose(fp);
    fp = NULL;

    return 0;
}

/* Writes the ASCII value of the character*/
void write_raw_byte(FILE *out, unsigned char ch)
{
    fputc((char)ch, out);
}

compressed_file *load_compressed_file(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        perror("Error: Failed to open file");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    compressed_file *cf = (compressed_file *)malloc(sizeof(compressed_file));
    cf->data = (unsigned char *)malloc(filesize);
    cf->data_bits = (int)(filesize * 8);
    cf->filename = strdup(filepath);

    fread(cf->data, 1, filesize, fp);
    fclose(fp);

    return cf;
}

void free_compressed_file(compressed_file *cf)
{
    free(cf->data);
    free(cf->filename);
    free(cf);
}
