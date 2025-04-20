#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "directory_parser.h"

char* build_filepath(const char *directory, char* filename)
{
    char* filepath = (char*)calloc(256, sizeof(char));
    strcpy(filepath, directory);
    strcat(filepath, "/");
    strcat(filepath, filename);
    return filepath;
}

int filename_has_ending(char *filename, const char *ending)
{
    size_t len = strlen(filename);
    size_t end_len = strlen(ending);
    if (len <= end_len)
        return 0;
    return strncmp(filename + len - end_len, ending, end_len) == 0;
}

int is_code_file(char *filename)
{
    return filename_has_ending(filename, ".c") || filename_has_ending(filename, ".h");
}

char* get_original_filename_from_bin(char* bin_filename)
{
    size_t suffix_len = strlen("_c.bin");
    size_t len = strlen(bin_filename);
    char* original_filename = NULL;
    int original_filename_len;
    if (len <= suffix_len) 
        return NULL;

    original_filename = (char*)calloc(256, sizeof(char));
    original_filename_len = len - suffix_len;
    strncpy(original_filename, bin_filename, original_filename_len);
    original_filename[original_filename_len] = '\0';
    return original_filename;
}

char* get_original_filename_from_c_or_h(char* c_or_h_filename)
{
    size_t suffix_len = strlen(".c");
    size_t len = strlen(c_or_h_filename);
    char* original_filename;
    int original_filename_len;
    if (len <= suffix_len) 
        return NULL;

    original_filename = (char*)calloc(256, sizeof(char));
    original_filename_len = len - suffix_len;
    strncpy(original_filename, c_or_h_filename, original_filename_len);
    original_filename[original_filename_len] = '\0';
    return original_filename;
}

char* convert_bin_to_c_or_h(char* bin_filename)
{
    /*input format: filename_c.bin or filename_h.bin
    output format: filename.c or filename.h
    */
    if(filename_has_ending(bin_filename, "_c.bin")){
        return convert_bin_to_c(bin_filename);
    } else if(filename_has_ending(bin_filename, "_h.bin")){
        return convert_bin_to_h(bin_filename);
    }
    return NULL;
}

char* convert_bin_to_c(char* bin_filename)
{
    char* filename = get_original_filename_from_bin(bin_filename);
    if (!filename) return NULL;
    strcat(filename, ".c");
    return filename;
}

char* convert_bin_to_h(char* bin_filename)
{
    char* filename = get_original_filename_from_bin(bin_filename);
    if (!filename) 
        return NULL;
    strcat(filename, ".h");
    return filename;
}

char* convert_c_or_h_to_bin(char* c_or_h_filename)
{
    if (filename_has_ending(c_or_h_filename, ".c")) {
        char* filename = get_original_filename_from_c_or_h(c_or_h_filename);
        strcat(filename, "_c.bin");
        return filename;
    } else if (filename_has_ending(c_or_h_filename, ".h")) {
        char* filename = get_original_filename_from_c_or_h(c_or_h_filename);
        strcat(filename, "_h.bin");
        return filename;
    }
    return NULL;
}

char* convert_c_to_bin(char* c_filename)
{
    char* filename = get_original_filename_from_c_or_h(c_filename);
    strcat(filename, "_c.bin");
    return filename;
}

char* convert_h_to_bin(char* h_filename)
{
    char* filename = get_original_filename_from_c_or_h(h_filename);
    strcat(filename, "_h.bin");
    return filename;
}

int get_file_length(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    int file_len;
    if (fp == NULL)
    {
        perror("Error: ");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    file_len = ftell(fp);
    fclose(fp);
    return file_len;
}

int read_input_file(unsigned char *buffer, int file_len, const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        perror("Error: ");
        return -1;
    }
    fread(buffer, 1, file_len, fp);
    buffer[file_len] = '\0';
    fclose(fp);
    return 0;
}

void write_raw_byte(FILE *out, unsigned char ch)
{
    fputc((char)ch, out);
}

compressed_file *load_compressed_file(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    long filesize;
    compressed_file *cf = NULL;
    if (fp == NULL)
    {
        perror("Error: Failed to open file");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);

    cf = (compressed_file *)malloc(sizeof(compressed_file));
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

compressed_file **load_multiple_compressed_files(const char *input_directory, int *file_count)
{
    int num_files_in_dir = get_num_files_in_directory(input_directory);
    size_t i;
    compressed_file **cfiles = NULL;
    char **filenames = NULL;
    char fullpath[1024];
    *file_count = num_files_in_dir;
    if (num_files_in_dir == 0)
        return NULL;

    cfiles = malloc(sizeof(compressed_file *) * num_files_in_dir);
    filenames = malloc(num_files_in_dir * sizeof(char *));
    for (i = 0; i < num_files_in_dir; i++)
    {
        /* assuming 256 is max filename length */
        filenames[i] = malloc(sizeof(char) * 256);
    }

    get_files_in_directory(input_directory, num_files_in_dir, filenames);

    for (i = 0; i < num_files_in_dir; i++)
    {
        /* Gets fullpath for each compressed file */
        snprintf(fullpath, sizeof(fullpath), "%s/%s", input_directory, filenames[i]);
        cfiles[i] = load_compressed_file(fullpath);
        free(filenames[i]);
    }
    free(filenames);

    return cfiles;
}

void free_multiple_compressed_files(compressed_file **cfiles, int file_count)
{
    size_t i;
    for (i = 0; i < file_count; i++)
    {
        free_compressed_file(cfiles[i]);
    }
    free(cfiles);
}
