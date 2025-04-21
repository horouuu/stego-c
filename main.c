#include <stdio.h>
#include <string.h>

/**
 * Main program for stego-c.
 * This program can perform 2 different functions: encoding c code files/directory into an image, and decoding c code files/directory from an image.
 * 
 * Flags:
 * `-h` or `--help`: (boolean) Show help message.
 * `-e` or `--encode`: (boolean) Enable encoding mode. Encode c code files/directory into an image. Cannot be used with decoding mode.
 * `-d` or `--decode`: (boolean) Enable decoding mode. Decode c code files/directory from an image. Cannot be used with encoding mode.
 * `-i`: (string) Input image file name with extension. Accepts PNG, JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) formats.
 * `-o`: (string) Output image file name with extension, only used for encoding. Accepts PNG, JPG/JPEG, BMP formats.
 * `-f`: (string) Input single c code file name with extension, only used for encoding.
 * `-fd`: (string) Input c code directory, only used for encoding.
 *
 * Example usage:
 * `./stego -e -i input.png -o output.png -f file.c`
 * `./stego -e -i input.png -o output.png -fd ./src`
 * `./stego -d -i input.png`
 */
int main(int argc, char **argsv) {
  /* Initialize variables */
  int i, mode_encoding = 0, mode_decoding = 0;
  char *input_image = NULL, *output_image = NULL, *input_file = NULL,
       *input_dir = NULL;

  /* Check empty args */
  if (argc < 2) {
    fprintf(
        stderr,
        "Usage: %s -e|-d -i <input image> -o <output image> -f <input file>\n",
        argsv[0]);
    return 1;
  }

  /* Parse args */
  for (i = 1; i < argc; i++) {
    if (argsv[i][0] == '-') {
      /* Match args using strcmp */
      if (strcmp(argsv[i], "-h") == 0 || strcmp(argsv[i], "--help") == 0) {
        printf("Usage: %s -e|-d -i <input image> -o <output image> -f <input "
               "file>\n",
               argsv[0]);
        printf("\nFlags:\n");
        printf("  -h, --help: Show this help message.\n");
        printf("  -e, --encode: Enable encoding mode. Encode c code "
               "files/directory into an image. Cannot be used with decoding "
               "mode.\n");
        printf("  -d, --decode: Enable decoding mode. Decode c code "
               "files/directory from an image. Cannot be used with encoding "
               "mode.\n");
        printf("  -i: Input image file name with extension. Accepts PNG, "
               "JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) "
               "formats.\n");
        printf("  -o: Output image file name with extension. Accepts PNG, "
               "JPG/JPEG, BMP formats. Used only in encoding.\n");
        printf("  -f: Input single c code file name with extension. Used only "
               "in encoding.\n");
        printf("  -fd: Input c code directory. Used only in encoding.\n");
        printf("\nExample usage:\n");
        printf("  %s -e -i input.png -o output.png -f file.c\n", argsv[0]);
        printf("  %s -d -i input.png -o output.c\n", argsv[0]);
        return 0;
      } else if (strcmp(argsv[i], "-e") == 0 ||
                 strcmp(argsv[i], "--encode") == 0) {
        mode_encoding = 1;
      } else if (strcmp(argsv[i], "-d") == 0 ||
                 strcmp(argsv[i], "--decode") == 0) {
        mode_decoding = 1;
      } else if (strcmp(argsv[i], "-i") == 0) {
        printf("Input image: %s\n", argsv[++i]);
        input_image = argsv[i];
      } else if (strcmp(argsv[i], "-o") == 0) {
        printf("Output image: %s\n", argsv[++i]);
        output_image = argsv[i];
      } else if (strcmp(argsv[i], "-f") == 0) {
        printf("Input code file: %s\n", argsv[++i]);
        input_file = argsv[i];
      } else if (strcmp(argsv[i], "-fd") == 0) {
        printf("Input code directory: %s\n", argsv[++i]);
        input_dir = argsv[i];
      } else {
        fprintf(stderr, "Error: Unknown argument: %s\n", argsv[i]);
        return 1;
      }
    }
  }

  /* Check args validity */
  if (mode_encoding && mode_decoding) {
    fprintf(stderr, "Error: Cannot use both encoding and decoding modes.\n");
    return 1;
  } else if (!mode_encoding && !mode_decoding) {
    fprintf(stderr, "Error: Must use either encoding or decoding mode.\n");
    return 1;
  } else if (mode_encoding &&
             (!input_image || !output_image || !input_file || !input_dir)) {
    fprintf(stderr, "Error: Missing required arguments for encoding mode.\n");
    fprintf(stderr,
            "Usage: %s -e -i <input image> -o <output image> -f <input file>\n",
            argsv[0]);
    return 1;
  } else if (mode_encoding && input_file && input_dir) {
    fprintf(stderr, "Error: Cannot use both input file and input directory.\n");
  } else if (mode_decoding && !input_image) {
    fprintf(stderr, "Error: Missing required arguments for decoding mode.\n");
    fprintf(stderr, "Usage: %s -d -i <input image>\n", argsv[0]);
    return 1;
  }

  return 0;
}
