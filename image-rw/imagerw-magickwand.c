#include "imagerw.h"
#include <MagickWand/MagickWand.h>

int load_image(const char *filename, ImageData *image) { return 1; }

int save_image(const char *filename, const ImageData *image) { return 1; }

unsigned char get_image_col_byte(int pos, const ImageData *image) {

  return image->data[pos];
}

int set_image_col_byte(int pos, ImageData *image, unsigned char value) {}

void cleanup_free_buffer(ImageData *image) {}