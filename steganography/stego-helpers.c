#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "stego-helpers.h"

const char *get_filename(const char *path)
{
    const char *slash;
    const char *backslash;
    const char *last_sep;
    if (path == NULL)
        return NULL;

    slash = strrchr(path, '/');
    backslash = strrchr(path, '\\');
    last_sep = slash > backslash ? slash : backslash;

    return (char *)last_sep ? (char *)last_sep + 1 : (char *)path;
}

void free_bitarray(BitArray *bitarray)
{
    free(bitarray->bit_array);
}

long bitarray_to_long(const BitArray bitarray)
{
    int i;
    long buffer = 0;

    for (i = 0; i < bitarray.num_bytes; i++)
    {
        buffer |= bitarray.bit_array[i] << (i * 8);
    }

    return buffer;
}

BitArray long_to_bitarray(const long num, const int pad_bits)
{
    BitArray out;
    unsigned char *filesize_bits_unsigned;
    unsigned char filesize_bit;
    int i;

    int j = 0;
    int k = 0;
    unsigned char bit_mask = 1;
    unsigned char filesize_buffer = 0;
    double bits = fmax(((log(num) / log(2)) + 1), pad_bits);
    int bytes = ceil(bits / 8);
    filesize_bits_unsigned = malloc(bytes * sizeof(char));

    for (i = 0; i < bits; i++)
    {
        filesize_bit = ((bit_mask << i) & num) >> i;
        filesize_buffer |= filesize_bit << j;
        j++;
        if (j == 8)
        {
            j = 0;
            filesize_bits_unsigned[k] = filesize_buffer;
            filesize_buffer = 0;
            k++;
        }
    }

    if ((int)bits % 8 != 0)
    {
        filesize_bits_unsigned[k] = filesize_buffer;
    }

    out.bit_array = filesize_bits_unsigned;
    out.num_bytes = bytes;

    return out;
}
