#include "../image-rw/imagerw.h"
#include "stego.h"
#include "stego-helpers.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void free_header_data(HeaderData *hd)
{
    free(hd->file_name);
}

int write_encoding_bytes(const unsigned char *data, const unsigned long data_size_bits, ImageData *image_data, const long start_pos)
{
    if (start_pos == -1)
    {
        printf("Error. Invalid start position.\n");
        return -1;
    }

    int bit_sig = 0;
    long max_img_idx = image_data->height * image_data->width - 1;
    long curr_pos = start_pos;
    long pos = start_pos;
    while (pos - max_img_idx > 0)
    {
        pos -= max_img_idx;
        bit_sig++;
    }

    unsigned char data_buffer;
    unsigned char data_bit;
    for (int i = 0; i < data_size_bits; i++)
    {
        if (pos > max_img_idx)
        {
            pos = 0;
            bit_sig++;
        }

        if (bit_sig > 7)
        {
            printf("Error! Could not write char at pos %d: Not enough space in image.\n", pos);
            return -1;
        }

        for (int j = 0; j < 8; j++)
        {
            data_bit = ((1 << j) & data[i]) >> j;
            data_buffer = get_image_col_byte(pos, image_data);
            if (data_bit == 1)
            {
                data_buffer |= (data_bit << bit_sig);
            }
            else
            {
                data_buffer &= (~1 << bit_sig) | (255 >> (8 - bit_sig));
            }

            set_image_col_byte(pos, image_data, data_buffer);
            pos++;
            curr_pos++;
        }
    }

    return curr_pos;
}

int encode_header(int end, int fns_bytes, const unsigned char *filename, unsigned long fs_bits, ImageData *image_data, unsigned long start_pos)
{
    BitArray fs_bitarray;
    const int SIZE_FILE = 24;
    if (fs_bits > 16777215)
    {
        printf("Error. Size of file is too large: %d", fs_bits);
        return -1;
    }

    if (fns_bytes > 256)
    {
        printf("Error. Size of file name is too large: %d", fns_bytes);
    }

    long pos = start_pos;

    unsigned char end_value = (unsigned char)end & 1;
    unsigned char end_buffer = get_image_col_byte(start_pos, image_data);
    if (end_value == 1)
    {
        end_buffer |= end_value;
    }
    else
    {
        end_buffer &= end_value;
    }
    set_image_col_byte(start_pos, image_data, end_buffer);
    pos++;
    pos = write_encoding_bytes((unsigned char *)&fns_bytes, 1, image_data, pos);

    fs_bitarray = long_to_bitarray(fs_bits, SIZE_FILE);

    pos = write_encoding_bytes(fs_bitarray.bit_array, 3, image_data, pos);
    pos = write_encoding_bytes(filename, fns_bytes, image_data, pos);

    free_bitarray(&fs_bitarray);
    return pos;
}

int encode_data(const unsigned char *data, int data_size_bits, const char *image_file_name, const char *output_file_name, unsigned long start_pos)
{
    ImageData image_data;
    unsigned long pos = start_pos;
    load_image(image_file_name, &image_data);

    pos = encode_header(1, 23, "my-hello_worldprogram.c", data_size_bits, &image_data, pos);

    save_image(output_file_name, &image_data);
    cleanup_free_buffer(&image_data);
    return 0;
}

HeaderData decode_header(ImageData *image_data, long start_pos)
{

    int end;
    int filename_size;
    long size;
    char *filename;

    int i;
    int j;

    unsigned char fns_read;
    unsigned char fs_read;
    unsigned char fn_read;
    HeaderData out;

    long pos = start_pos;
    const int MAX_POS_FILENAME_SIZE_BITS = 8;
    const int MAX_POS_SIZE_BITS = 24;

    end = get_image_col_byte(pos, image_data) & 1;
    printf("Last file: %s\n", end ? "true" : "false");
    pos++;

    filename_size = 0;
    for (i = 0; i < MAX_POS_FILENAME_SIZE_BITS; i++)
    {
        fns_read = get_image_col_byte(pos, image_data) & 1;
        filename_size |= fns_read << i;
        pos++;
    }
    printf("File name size: %dB\n", filename_size);
    unsigned long file_size = 0;
    for (j = 0; j < MAX_POS_SIZE_BITS; j++)
    {
        fs_read = get_image_col_byte(pos, image_data) & 1;
        file_size |= fs_read << j;
        pos++;
    }
    printf("File size: %db\n", file_size);

    int filename_limit = pos + filename_size * 8;
    unsigned char fn_buffer = 0;
    unsigned char fn_bit_counter = 0;
    unsigned char fn_byte_counter = 0;
    filename = malloc((filename_size + 1) * sizeof(char));
    while (pos < filename_limit)
    {
        fn_read = get_image_col_byte(pos, image_data) & 1;
        fn_buffer |= fn_read << fn_bit_counter;
        fn_bit_counter++;
        pos++;

        if (fn_bit_counter == 8)
        {
            filename[fn_byte_counter] = fn_buffer;
            fn_bit_counter = 0;
            fn_byte_counter++;
            fn_buffer = 0;
        }
    }
    filename[fn_byte_counter] = '\0';
    printf("File name: %s\n", filename);

    out.file_name = filename;
    out.file_name_size_bytes = filename_size;
    out.file_size_bits = file_size;
    out.last_file = end;

    return out;
}

int decode_image(const char *input_img_name, const char *output_file_name, ImageData *test)
{
    unsigned long pos = 105;
    unsigned char read;
    unsigned char read_buffer = 0;
    int byte_idx = 0;
    for (int j = 0; j < pos; j++)
    {
        read = get_image_col_byte(j, test);
        read_buffer |= ((1 & read) << byte_idx);
        if (byte_idx == 7)
        {
            byte_idx = 0;
            read_buffer = 0;
        }
        else
        {
            byte_idx++;
        }
    }
}

int main()
{
    unsigned char testData[5] = {'a', 'b', 'e', 'd', 'f'};
    encode_data(testData, 40, "input.png", "test.png", 0);

    ImageData test;
    load_image("test.png", &test);
    HeaderData d = decode_header(&test, 0);

    free_header_data(&d);
    cleanup_free_buffer(&test);
}