#include "keyword_map.h"
#include "decompressor.h"
#include "file_io.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN 64

void run_decompressor(const unsigned char *input, FILE *out){

    int curr_char;
    size_t i = 0;
    size_t token_i;
    char* output_keyword = malloc(sizeof(char) * MAX_TOKEN);
    while ((curr_char = input[i++]) != '\0'){
        // printf("curr_char: %c\n",curr_char);
        if(is_compressed_keyword(curr_char, output_keyword)){
            for(int j = 0 ; j < strlen(output_keyword) ; j ++){
                write_raw_byte(out, output_keyword[j]);
            }
        } else {
            write_raw_byte(out, curr_char);
        }
    }
}