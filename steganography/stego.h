#ifndef STEGO_H
#define STEGO_H
#include "../image-rw/imagerw.h"

typedef struct
{
    int last_file;
    int file_name_size_bytes;
    long file_size_bytes;
    char *file_name;
    unsigned long data_offset;
} HeaderData;

typedef struct
{
    long rel_pos;
    long true_pos;
    int bit_sig;
} OffsetData;

int write_encoding_bytes(const unsigned char *data, const unsigned long data_size, ImageData *image_data, const long start_pos);
HeaderData decode_header(ImageData *image_data, long start_pos);
void free_header_data(HeaderData *hd);

#endif