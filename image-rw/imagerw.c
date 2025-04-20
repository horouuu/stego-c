#include "imagerw.h"

int load_image(const char *filename, ImageData *image) {
  image->data =
      stbi_load(filename, &image->width, &image->height, &image->channels, 0);
  if (image->data == NULL) {
    fprintf(stderr, "Error loading image: %s\n", stbi_failure_reason());
    return 0;
  }
  return 1;
}

int save_image(const char *filename, ImageData *image);

unsigned char get_image_col_byte(int pos, ImageData *image) {
  if (pos < 0 || pos >= image->width * image->height * image->channels) {
    fprintf(stderr, "Error: Position out of bounds\n");
    return 0;
  }
  return image->data[pos];
}

int set_image_col_byte(int pos, ImageData *image, unsigned char value) {
  if (pos < 0 || pos >= image->width * image->height * image->channels) {
    fprintf(stderr, "Error: Position out of bounds\n");
    return 0;
  }
  image->data[pos] = value;
  return 1;
}