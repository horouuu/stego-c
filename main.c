#include <stdio.h>

/**
 * Main program for stego-c.
 * This program can perform 2 different functions: encoding c code files/directory into an image, and decoding c code files/directory from an image.
 * 
 * Flags:
 * `-h` or `--help`: (boolean) show help message.
 * `-e` or `--encode`: (boolean) encode c code files/directory into an image.
 * `-d` or `--decode`: (boolean) decode c code files/directory from an image.
 * `-i`: (string) input image file name with extension. Accepts PNG, JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) formats.
 * `-o`: (string) output image file name with extension, only used for encoding. Accepts PNG, JPG/JPEG, BMP formats.
 * `-f`: (string) input c code file name with extension/directory, only used for encoding.
 *
 * Example usage:
 * `./stego -e -i input.png -o output.png -f file.c`
 * `./stego -d -i input.png -o output.c`
 */
int main(int argc, char **argsv) {
  /* Initialize variables */
  int i, mode_encoding = 0, mode_decoding = 0;
  char *input_image = NULL, *output_image = NULL, *input_file = NULL;

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
      if (argsv[i][1] == 'h' || argsv[i][2] == 'h') {
        printf("Usage: %s -e|-d -i <input image> -o <output image> -f <input "
               "file>\n",
               argsv[0]);
        printf("\nFlags:\n");
        printf("  -h, --help: Show this help message.\n");
        printf(
            "  -e, --encode: Encode c code files/directory into an image.\n");
        printf(
            "  -d, --decode: Decode c code files/directory from an image.\n");
        printf("  -i: Input image file name with extension. Accepts PNG, "
               "JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) "
               "formats.\n");
        printf("  -o: Output image file name with extension. Accepts PNG, "
               "JPG/JPEG, BMP formats.\n");
        printf("  -f: Input c code file name with extension/directory.\n");
        printf("\nExample usage:\n");
        printf("  %s -e -i input.png -o output.png -f file.c\n", argsv[0]);
        printf("  %s -d -i input.png -o output.c\n", argsv[0]);
        return 0;
      } else if (argsv[i][1] == 'e' || argsv[i][2] == 'e') {
        mode_encoding = 1;
      } else if (argsv[i][1] == 'd' || argsv[i][2] == 'd') {
        mode_decoding = 1;
      } else if (argsv[i][1] == 'i' || argsv[i][2] == 'i') {
        printf("Input image: %s\n", argsv[++i]);
        input_image = argsv[i];
      } else if (argsv[i][1] == 'o' || argsv[i][2] == 'o') {
        printf("Output image: %s\n", argsv[++i]);
        output_image = argsv[i];
      } else if (argsv[i][1] == 'f' || argsv[i][2] == 'f') {
        printf("Input code file/directory: %s\n", argsv[++i]);
        input_file = argsv[i];
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
  } else if (mode_encoding && (!input_image || !output_image || !input_file)) {
    fprintf(stderr, "Error: Missing required arguments for encoding mode.\n");
    fprintf(stderr,
            "Usage: %s -e -i <input image> -o <output image> -f <input file>\n",
            argsv[0]);
    return 1;
  } else if (mode_decoding && !input_image) {
    fprintf(stderr, "Error: Missing required arguments for decoding mode.\n");
    fprintf(stderr, "Usage: %s -d -i <input image>\n", argsv[0]);
    return 1;
  }

  return 0;
}
