# stego-c

## Setup and Build

## Features

### File Reading

For encoding and decoding, a number of file formats can be read using the `stb_image.h` library, which is a single-header file library. These formats include PNG, JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) formats based on the file extension of the input file.

After encoding, the resulting image file is saved using the `stb_image_write.h` library, which can save to PNG, JPG/JPEG, BMP formats based on the file extension of the output file.

### Source Code Compression

Input files for compression are stored under `/compressor_input`.
Each file within this directory needs to be a `.c` file for the `compress_and_save` function to process it, otherwise the file is ignored.

Output files from compression are stored under `/compressor_output`
Each file within this directory will be a `.bin` file.

Input files for decompression are stored under `/compressor_output`, as the input for decompression is the output of compression

Output files from decompression are stored under `/decompressor_output`
Each file within this directory will be a `.c` file.

### Steganography

### Decoding

## Credits

STB (image file loading and saving): [https://github.com/nothings/stb/](https://github.com/nothings/stb/)
