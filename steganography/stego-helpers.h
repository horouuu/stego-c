#ifndef STEGO_HELPERS_H
#define STEGO_HELPERS_H

typedef struct
{
    unsigned char *bit_array;
    int num_bytes;
} BitArray;

void free_bitarray(BitArray *bitarray);
long bitarray_to_long(const BitArray bitarray);
BitArray long_to_bitarray(const long num, const int pad_bits);
double __cdecl fmax(double, double);

#endif