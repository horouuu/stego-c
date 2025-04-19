#include <stdio.h>
#include <string.h>
int get_file_length(const char *filepath)
{

    FILE *fp = NULL;
    fp = fopen(filepath, "rb");    
    if (fp == NULL)
    {
        perror("Error: ");
        return -1;
    }
    char c;
    int file_len = 0;
    while((c = fgetc(fp) != EOF)){
        file_len ++;
    }
    return file_len;
}
int read_input_file(char *buffer, int file_len, const char *filepath)
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