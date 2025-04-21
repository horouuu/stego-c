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

void free_stego_data(StegoData *sd)
{
    free(sd->data);
    free(sd->file_name);
}

void free_stego_data_collection(StegoDataCollection *sdc)
{
    int i;
    for (i = 0; i < sdc->num_files; i++)
    {
        free_stego_data(&sdc->output_data[i]);
    }

    free(sdc->output_data);
}

OffsetData get_offset_data(const long true_pos, const int img_max_idx)
{
    OffsetData out;
    int pixels_per_bit_sig = img_max_idx + 1;
    out.bit_sig = true_pos / pixels_per_bit_sig;
    out.rel_pos = true_pos % pixels_per_bit_sig;
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

    long max_img_idx = image_data->height * image_data->width * image_data->channels - 1;
    OffsetData offset_data = get_offset_data(true_start_pos, max_img_idx);
    int bit_sig = offset_data.bit_sig;
    long curr_pos = offset_data.true_pos;
    long pos = offset_data.rel_pos;

    unsigned char data_buffer;
    unsigned char data_bit;
    for (int i = 0; i < data_size_bytes; i++)
    {
        if (bit_sig > 7)
        {
            printf("Error! Could not write char at pos %d: Not enough space in image.\n", pos);
            cleanup_free_buffer(image_data);
            return -1;
        }

        for (int j = 0; j < 8; j++)
        {
            if (pos > max_img_idx)
            {
                pos = 0;
                bit_sig++;
            }

            data_bit = (data[i] >> j) & 1;
            data_buffer = get_image_col_byte(pos, image_data);
            if (data_bit == 1)
            {
                data_buffer |= (data_bit << bit_sig);
            }
            else
            {
                data_buffer &= ~(1 << bit_sig);
            }

            set_image_col_byte(pos, image_data, data_buffer);
            pos++;
            curr_pos++;
        }
    }

    printf("\n");

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

    int max_img_idx = image_data->height * image_data->width * image_data->channels - 1;
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
        end_buffer &= ~(1 << bit_sig);
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

int encode_data(const unsigned char *data, int data_size_bytes, const char *data_file_name, const char *image_file_name, const char *output_file_name, unsigned long true_start_pos, int last)
{
    ImageData image_data;
    unsigned long pos = true_start_pos;
    load_image(image_file_name, &image_data);
    pos = encode_header(last, strlen(data_file_name), data_file_name, data_size_bytes, &image_data, pos);
    if (pos == -1)
    {
        cleanup_free_buffer(&image_data);
        printf("Failed to encode header.");
        return -1;
    }

    pos = write_encoding_bytes(data, data_size_bytes, &image_data, pos);

    save_image(output_file_name, &image_data);
    cleanup_free_buffer(&image_data);
    return pos;
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

    int max_img_idx = image_data->height * image_data->width * image_data->channels - 1;
    offset_data = get_offset_data(true_start_pos, max_img_idx);
    int bit_sig = offset_data.bit_sig;
    long pos = offset_data.rel_pos;
    long true_pos = offset_data.true_pos;
    printf("--- FILE DATA ---\n");

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
        fns_read = (get_image_col_byte(pos, image_data) >> bit_sig) & 1;
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
        fs_read = (get_image_col_byte(pos, image_data) >> bit_sig) & 1;
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
        fn_read = (get_image_col_byte(pos, image_data) >> bit_sig) & 1;
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
    printf("\n");

    out.file_name = filename;
    out.file_name_size_bytes = filename_size;
    out.file_size_bytes = file_size;
    out.last_file = end;
    out.data_offset = true_pos;

    return out;
}

StegoDataCollection decode_image(const char *input_img_name)
{
    ImageData image_data;
    unsigned char read_bit;
    load_image(input_img_name, &image_data);
    int max_img_idx = image_data.height * image_data.width * image_data.channels - 1;
    int file_counter = 0;
    int last = 0;
    unsigned long true_pos = 0;
    StegoDataCollection out;
    StegoData *out_data = malloc(sizeof(StegoData) * 3);

    while (last == 0)
    {
        HeaderData d = decode_header(&image_data, true_pos);
        StegoData stego_buffer;

        true_pos = d.data_offset;
        last = d.last_file;
        unsigned long end_index = d.data_offset + d.file_size_bytes * 8;
        OffsetData offset_start = get_offset_data(true_pos, max_img_idx);
        OffsetData offset_end = get_offset_data(end_index, max_img_idx);

        int bit_sig = offset_start.bit_sig;
        int end_sig = offset_end.bit_sig;
        unsigned long rel_pos = offset_start.rel_pos;
        unsigned long end_pos = offset_end.rel_pos;
        unsigned char read_buffer = 0;
        unsigned char *data_buffer = malloc((d.file_size_bytes + 1) * sizeof(char));
        int bit_counter = 0;
        int byte_counter = 0;
        while ((rel_pos < end_pos) || (bit_sig < end_sig))
        {
            if (rel_pos > max_img_idx)
            {
                rel_pos = 0;
                bit_sig++;
            }

            if (bit_sig > 7)
            {
                printf("Error. End of file not found.");
                exit(1);
            }

            read_bit = (get_image_col_byte(rel_pos, &image_data) >> bit_sig) & 1;
            read_buffer |= (read_bit << bit_counter);
            bit_counter++;

            if (bit_counter == 8)
            {
                bit_counter = 0;
                data_buffer[byte_counter] = read_buffer;

                read_buffer = 0;
                byte_counter++;
            }

            rel_pos++;
            true_pos++;
        }

        stego_buffer.file_name = d.file_name;
        stego_buffer.file_name_size_bytes = d.file_name_size_bytes;
        stego_buffer.file_size_bytes = d.file_size_bytes;
        stego_buffer.true_pos = true_pos;
        stego_buffer.data = data_buffer;

        out_data[file_counter] = stego_buffer;
        file_counter++;
    }

    out.output_data = out_data;
    out.num_files = file_counter;
    cleanup_free_buffer(&image_data);

    return out;
}

int main()
{
    // Encoding
    compressed_file *c = load_compressed_file("./test/holy.txt");
    int length_bytes = c->data_bits / 8;
    int pos = encode_data(c->data, length_bytes, c->filename, "./test/forger.png", "./test/forger-test.png", 0, 1);
    free_compressed_file(c);

    // compressed_file *c2 = load_compressed_file("./test/input2_c.bin");
    // int lb = c2->data_bits / 8;
    // pos = encode_data(c2->data, lb, c2->filename, "./test/test.png", "./test/test2.png", pos, 1);
    // free_compressed_file(c2);

    // Decoding
    StegoDataCollection output_data = decode_image("./test/forger-test.png");
    for (int i = 0; i < output_data.num_files; i++)
    {
        save_compressed_file(output_data.output_data[i].data, output_data.output_data[i].file_size_bytes, "decoded.txt");
    }
    free_stego_data_collection(&output_data);
}