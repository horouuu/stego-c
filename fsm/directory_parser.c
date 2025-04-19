#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include "directory_parser.h"
int get_num_files_in_directory(const char* directory_path)
{
    struct dirent* entry;
    DIR* dir = opendir(directory_path);
    if(dir == NULL){
        perror("Error: could not open directory");
        return 1;
    }
    int num_files = 0;
    while((entry = readdir(dir)) != NULL){
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        num_files ++;
    }
    return num_files;
}

int get_files_in_directory(const char* directory_path, int num_files, char** filenames)
{
    struct dirent* entry;
    DIR* dir = opendir(directory_path);
    if(dir == NULL){
        perror("Error: could not open directory");
        return 1;
    }
    int i = 0;
    while((entry = readdir(dir)) != NULL && i < num_files){
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        printf("filepath: %s\n", entry->d_name);
        strcpy(filenames[i], entry->d_name);
        i++;
    }
    return 0;

}