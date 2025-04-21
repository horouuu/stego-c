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

typedef struct
{
    char *file_name;
    int file_name_size_bytes;
    long file_size_bytes;
    unsigned long true_pos;
    unsigned char *data;
} StegoData;

typedef struct
{
    StegoData *output_data;
    int num_files;
} StegoDataCollection;

void free_header_data(HeaderData *hd);
OffsetData get_offset_data(const long true_pos, const int img_max_idx);
int write_encoding_bytes(const unsigned char *data, const unsigned long data_size_bytes, ImageData *image_data, const long true_start_pos);
int encode_header(int end, int fns_bytes, const char *filename, unsigned long fs_bits, ImageData *image_data, unsigned long true_start_pos);
int encode_data(const unsigned char *data, int data_size_bits, const char *data_file_name, const char *image_file_name, const char *output_file_name, unsigned long true_start_pos, int last);
HeaderData decode_header(ImageData *image_data, long true_start_pos);
StegoDataCollection decode_image(const char *input_img_name);
void free_header_data(HeaderData *hd);
void free_stego_data_collection(StegoDataCollection *sdc);


#endif
