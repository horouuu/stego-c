#include "imagerw.h"
#include <stdlib.h>

/**
 * Loads an image, write random data to the 2 LSBs of each colour channel in the image, and saves it back to a file.
 * This is a test program for the image-rw library.
 * 
 * Run with the command:
 * `./test_imagerw <input filename> <output filename> <number of LSB>`
 *
 * Example:
 * `./test/_imagerw input.png output.png 2`
 */
int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr,
            "Usage: %s <input filename> <output filename> <number of LSB>\n",
            argv[0]);
    return 1;
  }
  const char *input_filename = argv[1];
  const char *output_filename = argv[2];
  const char *lsb = argv[3];
  int num_lsb = atoi(lsb);

  ImageData image;
  if (!load_image(input_filename, &image)) {
    return 1;
  }
  printf("Loaded image: %s (%d x %d, %d channels)\n", input_filename,
         image.width, image.height, image.channels);

  // Write random data to the LSBs for each colour channel of each pixel in the image
  char mask = ~(~0 << num_lsb); // Mask for the LSB channels
  for (int i = 0; i < image.width * image.height * image.channels; i++) {
    image.data[i] = (image.data[i] & ~mask) | (rand() % 256 & mask);
  }
  printf("Modified image data with random values in the %d LSBs of each colour "
         "channel.\n",
         num_lsb);

  // Save the modified image
  if (!save_image(output_filename, &image)) {
    cleanup_free_buffer(&image);
    return 1;
  }
  printf("Saved modified image: %s\n", output_filename);

  return 0;
}