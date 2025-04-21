# stego-c

Using Least Significant Bit (LSB) steganography to encode and decode entire c code files into and from an image, in c.

This program can perform 2 different functions: encoding c code files/directory into an image, and decoding c code files/directory from an image.

In the encoding step, the program reads and compresses all c code files using an elementary string replacement with a predefined map of keywords and their tokens. The lexer then encodes the code file metadata (e.g. file name, compressed byte size) into a fixed header structure followed by the binary data of the compressed file into the least significant bits (LSB) of each colour channel of each pixel in the input image.

In the case where the data to encode exceeds the number of bits encode-able in the LSB of the image, the lexer will move to the 2nd LSB of the pixel data, and so on and so forth until it reaches the MSB and no further encoding is possible.

In the decoding step, the program reads the encoded data, retrieving file metadata from the fixed header entries. The compressed data is then decompressed and reconstructed into a proper c code structure, with its filename and directory structure preserved.

## Usage

Encoding a single c code file into an image:

```bash
./stego -e -i input.png -o output.png -f file.c
```

Encoding a c source code directory into an image:

```bash
./stego -e -i input.png -o output.png -fd ./src
```

Decoding from an image:

```bash
./stego -d -i input.png
```

The decoded files will retain its original directory structure.

### Flags

- `-h` or `--help`: (boolean) Show help message.
- `-e` or `--encode`: (boolean) Enable encoding mode. Encode c code file into an image. Cannot be used with decoding mode.
- `-d` or `--decode`: (boolean) Enable decoding mode. Decode c code file from an image. Cannot be used with encoding mode.
- `-i`: (string) Input image file name with extension. Accepts PNG, JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) formats.
- `-o`: (string) Output image file name with extension, only used for encoding. Accepts PNG, JPG/JPEG, BMP formats.
- `-f`: (string) Input c code file name with extension, only used for encoding.

## Prerequisites

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
