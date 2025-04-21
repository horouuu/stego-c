#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "directory_parser.h"
#include "file_io.h"
int get_num_files_in_directory(const char *directory_path)
{
    struct dirent *entry;
    DIR *dir = opendir(directory_path);
    int num_files = 0;

    if (dir == NULL)
    {
        perror("Error: could not open directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        num_files++;
    }
    return num_files;
}

int get_files_in_directory(const char *directory_path, int num_files_in_dir, char **filenames)
{
    struct dirent *entry;
    DIR *dir = opendir(directory_path);
    size_t i = 0;
    if (dir == NULL)
    {
        perror("Error: could not open directory");
        return 0;
    }
    while ((entry = readdir(dir)) != NULL && i < num_files_in_dir)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        printf("filepath: %s\n", entry->d_name);
        strcpy(filenames[i], entry->d_name);
        i++;
    }
    return 1;
}

int get_code_files_in_directory(const char *directory_path, int num_files_in_dir, char **code_filenames)
{
    struct dirent *entry;
    DIR *dir = opendir(directory_path);
    size_t i = 0;
    if (dir == NULL)
    {
        perror("Error: could not open directory");
        return 0;
    }
    while ((entry = readdir(dir)) != NULL && i < num_files_in_dir)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !is_code_file(entry->d_name))
            continue;
        printf("filepath: %s\n", entry->d_name);
        strcpy(code_filenames[i], entry->d_name);
        i++;
    }
    return 1;
}

