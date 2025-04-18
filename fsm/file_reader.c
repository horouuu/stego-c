#include <stdio.h>
#include <string.h>
int read_input_file(char *buffer, const char *filepath)
{
    FILE *fp = NULL;
    // int buffer_offset = 0;
    fp = fopen(filepath, "r");
    // char* line_buffer = (char*) malloc(100 * sizeof(char));
    if (fp == NULL)
    {
        perror("Error: ");
        return -1;
    }
    size_t bytes_read = fread(buffer, 1, 999, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);
    fp = NULL;

    return 0;
}