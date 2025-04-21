#include "../image-rw/imagerw.h"
#include "stego.h"
#include "stego-helpers.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../fsm/file_io.h"

void free_header_data(HeaderData *hd)
{
    free(hd->file_name);
}

OffsetData get_offset_data(const long true_pos, const int img_max_idx)
{
    OffsetData out;
    long rel_pos = true_pos;
    int bit_sig = 0;
    while (rel_pos - img_max_idx > 0)
    {
        rel_pos -= img_max_idx;
        bit_sig++;
    }

    out.bit_sig = bit_sig;
    out.rel_pos = rel_pos;
    out.true_pos = true_pos;

    return out;
}

int write_encoding_bytes(const unsigned char *data, const unsigned long data_size_bytes, ImageData *image_data, const long true_start_pos)
{
    if (true_start_pos == -1)
    {
        printf("Error. Invalid start position.\n");
        cleanup_free_buffer(image_data);
        return -1;
    }

    long max_img_idx = image_data->height * image_data->width - 1;
    OffsetData offset_data = get_offset_data(true_start_pos, max_img_idx);
    int bit_sig = offset_data.bit_sig;
    long curr_pos = offset_data.true_pos;
    long pos = offset_data.rel_pos;

    unsigned char data_buffer;
    unsigned char data_bit;
    for (int i = 0; i < data_size_bytes; i++)
    {
        if (pos > max_img_idx)
        {
            pos = 0;
            bit_sig++;
        }

        if (bit_sig > 7)
        {
            printf("Error! Could not write char at pos %d: Not enough space in image.\n", pos);
            cleanup_free_buffer(image_data);
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

int encode_header(int end, int fns_bytes, const unsigned char *filename, unsigned long fs_bits, ImageData *image_data, unsigned long true_start_pos)
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
        return -1;
    }

    int max_img_idx = image_data->height * image_data->width - 1;
    OffsetData offset_data = get_offset_data(true_start_pos, max_img_idx);
    long pos = offset_data.rel_pos;
    long true_pos = offset_data.true_pos;
    int bit_sig = offset_data.bit_sig;

    unsigned char end_value = (unsigned char)end & 1;
    unsigned char end_buffer = get_image_col_byte(true_start_pos, image_data);
    if (end_value == 1)
    {
        end_buffer |= (end_value << bit_sig);
    }
    else
    {
        end_buffer &= (~1 << bit_sig) | (255 >> (8 - bit_sig));
    }
    set_image_col_byte(true_start_pos, image_data, end_buffer);
    pos++;
    true_pos++;

    true_pos = write_encoding_bytes((unsigned char *)&fns_bytes, 1, image_data, true_pos);

    fs_bitarray = long_to_bitarray(fs_bits, SIZE_FILE);

    true_pos = write_encoding_bytes(fs_bitarray.bit_array, 3, image_data, true_pos);
    true_pos = write_encoding_bytes(filename, fns_bytes, image_data, true_pos);

    free_bitarray(&fs_bitarray);
    return true_pos;
}

int encode_data(const unsigned char *data, int data_size_bits, const char *data_file_name, const char *image_file_name, const char *output_file_name, unsigned long true_start_pos)
{
    ImageData image_data;
    unsigned long pos = true_start_pos;
    load_image(image_file_name, &image_data);
    pos = encode_header(1, strlen(data_file_name), data_file_name, data_size_bits, &image_data, pos);
    if (pos == -1)
    {
        cleanup_free_buffer(&image_data);
        printf("Failed to encode header.");
        return -1;
    }

    write_encoding_bytes(data, data_size_bits, &image_data, pos);

    save_image(output_file_name, &image_data);
    cleanup_free_buffer(&image_data);
    return 0;
}

HeaderData decode_header(ImageData *image_data, long true_start_pos)
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
    OffsetData offset_data;

    const int MAX_POS_FILENAME_SIZE_BITS = 8;
    const int MAX_POS_SIZE_BITS = 24;

    int max_img_idx = image_data->height * image_data->width - 1;
    offset_data = get_offset_data(true_start_pos, max_img_idx);
    int bit_sig = offset_data.bit_sig;
    long pos = offset_data.rel_pos;
    long true_pos = offset_data.true_pos;

    end = get_image_col_byte(pos, image_data) & (1 << bit_sig);
    printf("Last file: %s\n", end ? "true" : "false");
    pos++;
    true_pos++;
    if (pos > max_img_idx)
    {
        pos = 0;
        bit_sig++;
    }

    filename_size = 0;
    for (i = 0; i < MAX_POS_FILENAME_SIZE_BITS; i++)
    {
        fns_read = get_image_col_byte(pos, image_data) & (1 << bit_sig);
        filename_size |= fns_read << i;
        pos++;
        true_pos++;
        if (pos > max_img_idx)
        {
            pos = 0;
            bit_sig++;
        }
    }
    printf("File name size: %dB\n", filename_size);
    unsigned long file_size = 0;
    for (j = 0; j < MAX_POS_SIZE_BITS; j++)
    {
        fs_read = get_image_col_byte(pos, image_data) & (1 << bit_sig);
        file_size |= fs_read << j;
        pos++;
        true_pos++;
        if (pos > max_img_idx)
        {
            pos = 0;
            bit_sig++;
        }
    }
    printf("File size: %dB\n", file_size);

    int filename_limit = pos + filename_size * 8;
    unsigned char fn_buffer = 0;
    unsigned char fn_bit_counter = 0;
    unsigned char fn_byte_counter = 0;
    filename = malloc((filename_size + 1) * sizeof(char));
    while (pos < filename_limit)
    {
        fn_read = get_image_col_byte(pos, image_data) & (1 << bit_sig);
        fn_buffer |= fn_read << fn_bit_counter;
        fn_bit_counter++;
        pos++;
        true_pos++;

        if (pos > max_img_idx)
        {
            pos = 0;
            bit_sig++;
        }

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
    out.file_size_bytes = file_size;
    out.last_file = end;
    out.data_offset = true_pos;

    return out;
}

int decode_image(const char *input_img_name, unsigned char *output)
{
    ImageData image_data;
    unsigned char read_bit;
    load_image(input_img_name, &image_data);
    HeaderData d = decode_header(&image_data, 0);
    unsigned long pos = d.data_offset;
    unsigned long end_index = d.data_offset + d.file_size_bytes * 8;

    int bit_sig = 0;
    int max_img_idx = image_data.width * image_data.height - 1;
    unsigned char read_buffer = 0;
    unsigned char *data_buffer = malloc((d.file_size_bytes + 1) * sizeof(char));
    int bit_counter = 0;
    int byte_counter = 0;
    for (pos; pos < end_index; pos++)
    {
        read_bit = get_image_col_byte(pos, &image_data) & (1 << bit_sig);
        read_buffer |= (read_bit << bit_counter);
        bit_counter++;

        if (bit_counter == 8)
        {
            bit_counter = 0;
            data_buffer[byte_counter] = read_buffer;
            read_buffer = 0;
            byte_counter++;
        }
    }

    memcpy(output, data_buffer, byte_counter);
    free_header_data(&d);
    free(data_buffer);
    cleanup_free_buffer(&image_data);
}

int main()
{
    compressed_file *c = load_compressed_file("./test/input1.c.bin");
    unsigned char *testData = c->data;
    encode_data(c->data, c->data_bits / 8, c->filename, "./test/input.png", "./test/test.png", 0);
    free_compressed_file(c);

    unsigned char *output = malloc(((c->data_bits / 8) + 1) * sizeof(char));
    output[c->data_bits / 8] = '\0';
    decode_image("./test/test.png", output);

    while (*output && output != '\0')
    {
        printf("%c", *output);
        output++;
    }

    free(output);
}